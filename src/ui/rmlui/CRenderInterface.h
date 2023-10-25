#pragma once
#include "pch.h"

namespace ui
{
    class CRenderInterface : public Rml::RenderInterface
    {
    private:
        struct CompiledGeometry
        {
            bgfx::VertexBufferHandle m_vertex_buffer = BGFX_INVALID_HANDLE;
            bgfx::IndexBufferHandle m_index_buffer = BGFX_INVALID_HANDLE;
            Rml::TextureHandle m_texture = 0;
        };

        bool m_enable_scissor_region = false;
        Eigen::Vector4i m_scissor_region;

        bgfx::ProgramHandle m_textured_program;
        bgfx::VertexLayout m_rml_vertex_layout{};
        bgfx::TextureHandle white_texture;

        bgfx::UniformHandle m_translation_handle = BGFX_INVALID_HANDLE;
        bgfx::UniformHandle m_projection_handle = BGFX_INVALID_HANDLE;
        bgfx::UniformHandle m_transform_handle = BGFX_INVALID_HANDLE;
        bgfx::UniformHandle m_sampler = BGFX_INVALID_HANDLE;

        Rml::CompiledGeometryHandle m_compiled_geometry_counter = 0;
        std::unordered_map<Rml::CompiledGeometryHandle, CompiledGeometry> m_compiled_geometry;

        Rml::TextureHandle m_texture_counter = 0;
        std::unordered_map<Rml::TextureHandle, bgfx::TextureHandle> m_textures;

        bgfx::ViewId m_render_view_id = 0;
        
        Eigen::Matrix4f m_projection;

        int m_width, m_height;

        uint32_t m_idx = 0;

    public:
        CRenderInterface(int width, int height);

        void SetupProjection();
        void Resize(int width, int height);
        void Render(const Rml::Vector2f& translation, bgfx::VertexBufferHandle vertex_buffer, bgfx::IndexBufferHandle index_buffer, Rml::TextureHandle texture, uint64_t state);

        void RenderGeometry(Rml::Vertex* vertices, int numVertices, int* indices, int numIndices, Rml::TextureHandle texture, const Rml::Vector2f& translation) override;

        Rml::CompiledGeometryHandle CompileGeometry(Rml::Vertex* vertices, int numVertices, int* indices, int numIndices, Rml::TextureHandle texture) override;
        void RenderCompiledGeometry(Rml::CompiledGeometryHandle geometry, const Rml::Vector2f& translation) override;
        void ReleaseCompiledGeometry(Rml::CompiledGeometryHandle geometry) override;

        bool LoadTexture(Rml::TextureHandle& textureHandle, Rml::Vector2i& textureDimensions, const Rml::String& source) override;
        bool GenerateTexture(Rml::TextureHandle& textureHandle, const uint8_t* source, const Rml::Vector2i& sourceDimensions) override;
        void ReleaseTexture(Rml::TextureHandle texture) override;

        void EnableScissorRegion(bool enable) override;
        void SetScissorRegion(int x, int y, int width, int height) override;

        void SetTransform(const Rml::Matrix4f* transform) override;
    };
}