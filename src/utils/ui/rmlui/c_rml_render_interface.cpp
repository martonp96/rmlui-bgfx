#include "c_rml_render_interface.h"
#include "rml_helpers.h"

#include <fstream>
#include <bgfx/bgfx.h>
#include <bgfx/embedded_shader.h>
#include <bx/math.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

using namespace utils;

const bgfx::Memory* read_file(const std::string& path)
{
    std::ifstream file(path.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
    if(!file.is_open() || !file)
    {
        printf("failed to open file %s\n", path.c_str());
        return nullptr;
    }

    uint32_t size = file.tellg();
    auto out_mem = bgfx::alloc(size);
    file.seekg(0);
    file.read((char*)out_mem->data, out_mem->size);
    file.close();
    return out_mem;
}

ui::c_rml_render_interface::c_rml_render_interface(int width, int height) : m_width(width), m_height(height)
{
    m_render_view_id = 0;
    
    m_rml_vertex_layout
        .begin()
        .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .end();

    auto vertMemory = read_file(R"(D:\repos\bgfxtest\data\shaders\vert.bin)");
    bgfx::ShaderHandle vertex_shader = bgfx::createShader(vertMemory);

    auto texturedMemory = read_file(R"(D:\repos\bgfxtest\data\shaders\textured_frag.bin)");
    bgfx::ShaderHandle textured_frag = bgfx::createShader(texturedMemory);

    m_textured_program = bgfx::createProgram(vertex_shader, textured_frag, false);

    auto mem = bgfx::alloc(4);
    std::memset(mem->data, 0xFF, 4);
    white_texture = bgfx::createTexture2D(1, 1, false, 1, bgfx::TextureFormat::RGBA8, 0, mem);

    m_projection_handle = createUniform("u_projection", bgfx::UniformType::Mat4);
    m_transform_handle = createUniform("u_transform", bgfx::UniformType::Mat4);
    m_translation_handle = createUniform("u_translate", bgfx::UniformType::Vec4);
    m_sampler = createUniform("s_texColor", bgfx::UniformType::Sampler);

    //init and set transform data once for the shader
    m_transform.setIdentity();
    bgfx::setUniform(m_transform_handle, m_transform.data());

    bx::mtxOrtho(m_projection.data(), 0.f, m_width, m_height, 0.f, -10000.f, 10000.f, 0.f, bgfx::getCaps()->homogeneousDepth);
    bgfx::setUniform(m_projection_handle, m_projection.data());
}

void ui::c_rml_render_interface::resize(int width, int height)
{
    m_width = width;
    m_height = height;

    bx::mtxOrtho(m_projection.data(), 0.f, m_width, m_height, 0.f, -10000.f, 10000.f, 0.f, bgfx::getCaps()->homogeneousDepth);
    bgfx::setUniform(m_projection_handle, m_projection.data());
}

Rml::CompiledGeometryHandle ui::c_rml_render_interface::CompileGeometry(Rml::Vertex* vertices, int numVertices,	int* indices, int numIndices, Rml::TextureHandle texture)
{
    //printf("%s\n", __FUNCTION__);

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

void ui::c_rml_render_interface::RenderCompiledGeometry(Rml::CompiledGeometryHandle geometryHandle, const Rml::Vector2f& translation)
{
    const auto geometryIter = m_compiled_geometry.find(geometryHandle);
    if (geometryIter == m_compiled_geometry.end())
        return;

    const auto& geometry = *geometryIter;

    set_transform_uniform(translation);
    bgfx::setVertexBuffer(0, geometry.second.m_vertex_buffer);
    bgfx::setIndexBuffer(geometry.second.m_index_buffer);
    bgfx::setState(0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA));

    update_scissor();
    set_texture(geometry.second.m_texture);
    bgfx::submit(m_render_view_id, m_textured_program);
}

void ui::c_rml_render_interface::ReleaseCompiledGeometry(Rml::CompiledGeometryHandle geometryHandle)
{
    //printf("%s\n", __FUNCTION__);
    const auto geometryIter = m_compiled_geometry.find(geometryHandle);
    if (geometryIter == m_compiled_geometry.end())
        return;

    const auto& geometry = *geometryIter;
    bgfx::destroy(geometry.second.m_vertex_buffer);
    bgfx::destroy(geometry.second.m_index_buffer);
    m_compiled_geometry.erase(geometryIter);
}

void ui::c_rml_render_interface::RenderGeometry(Rml::Vertex* vertices, int numVertices, int* indices, int numIndices, Rml::TextureHandle texture, const Rml::Vector2f& translation)
{
    //printf("%s\n", __FUNCTION__);
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

    set_transform_uniform(translation);
    bgfx::setVertexBuffer(0, &vb);
    bgfx::setIndexBuffer(&ib);
    bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_BLEND_ALPHA);

    update_scissor();
    set_texture(texture);
    bgfx::submit(m_render_view_id, m_textured_program);
}

bool ui::c_rml_render_interface::LoadTexture(Rml::TextureHandle& textureHandle, Rml::Vector2i& textureDimensions, const Rml::String& source)
{
    //printf("%s\n", __FUNCTION__);

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

bool ui::c_rml_render_interface::GenerateTexture(Rml::TextureHandle& textureHandle, const uint8_t* source, const Rml::Vector2i& sourceDimensions)
{
    //printf("%s\n", __FUNCTION__);
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

void ui::c_rml_render_interface::ReleaseTexture(Rml::TextureHandle texture)
{
    //printf("%s\n", __FUNCTION__);
    const auto textureIter = m_textures.find(texture);
    if (textureIter == m_textures.end())
        return;

	bgfx::destroy(textureIter->second);

    m_textures.erase(textureIter);
}

void ui::c_rml_render_interface::EnableScissorRegion(bool enable)
{
    //printf("%s - %d\n", __FUNCTION__, enable);
	m_enable_scissor_region = enable;
}

void ui::c_rml_render_interface::SetScissorRegion(int x, int y, int width, int height)
{
    //printf("%s %d %d %d %d\n", __FUNCTION__, x, y, width, height);
    m_scissor_region = { x, y, width, height };
}

void ui::c_rml_render_interface::SetTransform(const Rml::Matrix4f* transform)
{
    if (!transform)
    {
        m_transform.setIdentity();
    }
    else
    {
        std::memcpy(m_transform.data(), transform->data(), sizeof(Eigen::Matrix4f));
    }
    bgfx::setUniform(m_transform_handle, m_transform.data());
}

void ui::c_rml_render_interface::set_transform_uniform(const Rml::Vector2f& translation)
{
    Eigen::Vector4f trans(translation[0], translation[1], 0.f, 0.f);
    bgfx::setUniform(m_translation_handle, trans.data());
}

void ui::c_rml_render_interface::set_texture(Rml::TextureHandle texture)
{
    if (auto textureIter = m_textures.find(texture); textureIter != m_textures.end())
        bgfx::setTexture(0, m_sampler, textureIter->second);
    else bgfx::setTexture(0, m_sampler, white_texture);
}

void ui::c_rml_render_interface::update_scissor()
{
    if (m_enable_scissor_region)
    {
        bgfx::setScissor(
            static_cast<uint16_t>(m_scissor_region[0]),
            static_cast<uint16_t>(m_scissor_region[1]),
            static_cast<uint16_t>(m_scissor_region[2]),
            static_cast<uint16_t>(m_scissor_region[3]));
    }
    else
    {
        bgfx::setViewScissor(m_render_view_id, 0, 0, 0, 0);
    }
}
