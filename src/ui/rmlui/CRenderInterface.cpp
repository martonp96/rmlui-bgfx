#include "CRenderInterface.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

static unsigned char g_vertex_shader[] = {
    #include "vert.bin.h"
};

static unsigned char g_fragment_shader[] = {
    #include "frag.bin.h"
};

ui::CRenderInterface::CRenderInterface(const Eigen::Vector2i& size) : m_size(size)
{
    m_render_view_id = 0;

    //create the shaders for rmlui
    m_rml_vertex_layout
        .begin()
        .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .end();

    auto vertMemory = bgfx::copy(g_vertex_shader, sizeof(g_vertex_shader));
    bgfx::ShaderHandle vertex_shader = bgfx::createShader(vertMemory);

    auto fragMemory = bgfx::copy(g_fragment_shader, sizeof(g_fragment_shader));
    bgfx::ShaderHandle fragment_shader = bgfx::createShader(fragMemory);

    m_textured_program = bgfx::createProgram(vertex_shader, fragment_shader, false);

    //create a blank white texture that will be used when rmlui can't find the textures
    auto mem = bgfx::alloc(4);
    std::memset(mem->data, 0xFF, 4);
    white_texture = bgfx::createTexture2D(1, 1, false, 1, bgfx::TextureFormat::RGBA8, 0, mem);

    //init the uniforms which will be passed to the shader
    m_projection_handle = bgfx::createUniform("u_projection", bgfx::UniformType::Mat4);
    m_transform_handle = bgfx::createUniform("u_transform", bgfx::UniformType::Mat4);
    m_translation_handle = bgfx::createUniform("u_translate", bgfx::UniformType::Vec4);
    m_sampler = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);

    //init and set transform data once
    bgfx::setUniform(m_transform_handle, Rml::Matrix4f::Identity().data());

    SetupProjection();
}

Rml::CompiledGeometryHandle ui::CRenderInterface::CompileGeometry(Rml::Vertex* vertices, int numVertices, int* indices, int numIndices, Rml::TextureHandle texture)
{
    const bgfx::Memory* vertexMemory = bgfx::alloc(m_rml_vertex_layout.getSize(numVertices));
    const bgfx::Memory* indexMemory = bgfx::alloc(numIndices * sizeof(int));

    std::memcpy(vertexMemory->data, vertices, m_rml_vertex_layout.getSize(numVertices));
    std::memcpy(indexMemory->data, indices, numIndices * sizeof(int));

    const auto vertexBuffer = bgfx::createVertexBuffer(vertexMemory, m_rml_vertex_layout);
    const auto indexBuffer = bgfx::createIndexBuffer(indexMemory, BGFX_BUFFER_INDEX32);

    const auto handle = ++m_compiled_geometry_counter;
    m_compiled_geometry[handle] =
    {
        vertexBuffer,
        indexBuffer,
        texture
    };

    return handle;
}

void ui::CRenderInterface::RenderCompiledGeometry(Rml::CompiledGeometryHandle geometryHandle, const Rml::Vector2f& translation)
{
    const auto geometryIter = m_compiled_geometry.find(geometryHandle);
    if (geometryIter == m_compiled_geometry.end())
        return;

    const auto& geometry = *geometryIter;

    Render(
        translation, 
        geometry.second.m_vertex_buffer, 
        geometry.second.m_index_buffer,
        geometry.second.m_texture,
        0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA)
    );
}

void ui::CRenderInterface::ReleaseCompiledGeometry(Rml::CompiledGeometryHandle geometryHandle)
{
    const auto geometryIter = m_compiled_geometry.find(geometryHandle);
    if (geometryIter == m_compiled_geometry.end())
        return;

    const auto& geometry = *geometryIter;
    bgfx::destroy(geometry.second.m_vertex_buffer);
    bgfx::destroy(geometry.second.m_index_buffer);
    m_compiled_geometry.erase(geometryIter);
}

void ui::CRenderInterface::RenderGeometry(Rml::Vertex* vertices, int numVertices, int* indices, int numIndices, Rml::TextureHandle texture, const Rml::Vector2f& translation)
{
    if (bgfx::getAvailTransientVertexBuffer(numVertices, m_rml_vertex_layout) != static_cast<uint32_t>(numVertices) ||
        bgfx::getAvailTransientIndexBuffer(numIndices, true) != static_cast<uint32_t>(numIndices))
    {
        return;
    }

    bgfx::TransientVertexBuffer vb{};
    bgfx::TransientIndexBuffer ib{};

    bgfx::allocTransientVertexBuffer(&vb, numVertices, m_rml_vertex_layout);
    bgfx::allocTransientIndexBuffer(&ib, numIndices, true);

    std::memcpy(vb.data, vertices, m_rml_vertex_layout.getSize(numVertices));
    std::memcpy(ib.data, indices, numIndices * sizeof(int));

    Render(
        translation, 
        vb.handle, 
        ib.handle,
        texture,
        BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_BLEND_ALPHA
    );
}

bool ui::CRenderInterface::LoadTexture(Rml::TextureHandle& textureHandle, Rml::Vector2i& textureDimensions, const Rml::String& source)
{
    const auto file_interface = Rml::GetFileInterface();
    const auto file_handle = file_interface->Open(source);

    if (!file_handle)
        return false;

    std::vector<uint8_t> file_data(file_interface->Length(file_handle));
    file_interface->Read(file_data.data(), file_data.size(), file_handle);
    file_interface->Close(file_handle);

    std::uint8_t* data{ stbi_load_from_memory(file_data.data(), file_data.size(), &textureDimensions.x, &textureDimensions.y, nullptr, 4) };
    if (!data)
        return false;

    GenerateTexture(textureHandle, data, textureDimensions);

    stbi_image_free(data);
    return textureHandle;
}

bool ui::CRenderInterface::GenerateTexture(Rml::TextureHandle& textureHandle, const uint8_t* source, const Rml::Vector2i& sourceDimensions)
{
    constexpr uint32_t c_stride = 4;
    const uint32_t size = c_stride * sourceDimensions.x * sourceDimensions.y;

    if (!bgfx::isTextureValid(0, false, 1, bgfx::TextureFormat::RGBA8, 0))
        return false;

    const bgfx::Memory* mem = bgfx::alloc(size);
    std::memcpy(mem->data, source, mem->size);

    const auto texHandle = bgfx::createTexture2D(static_cast<uint16_t>(sourceDimensions.x), static_cast<uint16_t>(sourceDimensions.y), false, 1, bgfx::TextureFormat::RGBA8, 0, mem);

    if (!bgfx::isValid(texHandle))
        return false;

    textureHandle = ++m_texture_counter;
    m_textures[textureHandle] = texHandle;

    return true;
}

void ui::CRenderInterface::ReleaseTexture(Rml::TextureHandle texture)
{
    const auto textureIter = m_textures.find(texture);
    if (textureIter == m_textures.end())
        return;

    bgfx::destroy(textureIter->second);

    m_textures.erase(textureIter);
}

void ui::CRenderInterface::EnableScissorRegion(bool enable)
{
    m_enable_scissor_region = enable;
}

void ui::CRenderInterface::SetScissorRegion(int x, int y, int width, int height)
{
    m_scissor_region = { x, y, width, height };
}

void ui::CRenderInterface::SetTransform(const Rml::Matrix4f* transform)
{
    bgfx::setUniform(m_transform_handle, transform ? transform->data() : Rml::Matrix4f::Identity().data());
}

void ui::CRenderInterface::SetupProjection()
{
    bx::mtxOrtho(m_projection.data(), 0.f, m_size[0], m_size[1], 0.f, -10000.f, 10000.f, 0.f, bgfx::getCaps()->homogeneousDepth);
    bgfx::setUniform(m_projection_handle, m_projection.data());
}

void ui::CRenderInterface::Resize(const Eigen::Vector2i& size)
{
    m_size = size;
    SetupProjection();
}

void ui::CRenderInterface::Render(const Rml::Vector2f& translation, bgfx::VertexBufferHandle vertex_buffer, bgfx::IndexBufferHandle index_buffer, Rml::TextureHandle texture, uint64_t state)
{
    bgfx::setVertexBuffer(0, vertex_buffer);
    bgfx::setIndexBuffer(index_buffer);
    bgfx::setState(state);

    Eigen::Vector4f trans(translation[0], translation[1], 0.f, 0.f);
    bgfx::setUniform(m_translation_handle, trans.data());

    if (m_enable_scissor_region)
    {
        bgfx::setScissor(m_scissor_region[0], m_scissor_region[1], m_scissor_region[2], m_scissor_region[3]);
    }
    else
    {
        bgfx::setViewScissor(m_render_view_id);
    }

    if (auto textureIter = m_textures.find(texture); textureIter != m_textures.end())
        bgfx::setTexture(0, m_sampler, textureIter->second);
    else bgfx::setTexture(0, m_sampler, white_texture);

    bgfx::submit(m_render_view_id, m_textured_program);
}
