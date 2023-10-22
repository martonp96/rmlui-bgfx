#pragma once
#include "main.h"

#include <RmlUi/Core.h>
#include <bgfx/bgfx.h>
#include <Eigen/Eigen>

namespace utils::ui
{
	class c_rml_render_interface : public Rml::RenderInterface
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

		Eigen::Matrix4f m_transform;
		Eigen::Matrix4f m_projection;

        int m_width, m_height;

		uint32_t m_idx = 0;

	public:
		c_rml_render_interface(int width, int height);
        void resize(int width, int height);

		void RenderGeometry(Rml::Vertex* vertices, int numVertices, int* indices, int numIndices, Rml::TextureHandle texture, const Rml::Vector2f& translation) override;

		Rml::CompiledGeometryHandle CompileGeometry(Rml::Vertex* vertices, int numVertices, int* indices, int numIndices, Rml::TextureHandle texture) override;
		void RenderCompiledGeometry(Rml::CompiledGeometryHandle geometry, const Rml::Vector2f& translation) override;
		void ReleaseCompiledGeometry(Rml::CompiledGeometryHandle geometry) override;

		bool LoadTexture(Rml::TextureHandle& textureHandle, Rml::Vector2i& textureDimensions, const Rml::String& source) override;
		bool GenerateTexture(Rml::TextureHandle& textureHandle, const uint8_t* source, const Rml::Vector2i& sourceDimensions) override;
		void ReleaseTexture(Rml::TextureHandle texture);

		void EnableScissorRegion(bool enable) override;
		void SetScissorRegion(int x, int y, int width, int height) override;

		void SetTransform(const Rml::Matrix4f* transform) override;

		void set_transform_uniform(const Rml::Vector2f& translation);
		void set_texture(Rml::TextureHandle texture);
		void update_scissor();

		bool scissor_enabled() { return m_enable_scissor_region; }
	};
}