#pragma once
#include "Scene.h"
#include "World/World.h"
#include "Renderer/GrassMesh.h"
#include "Renderer/RenderTile.h"
#include "Util/Util.h"
#include "Util/Framebuffer.h"

class Renderer
{
public:
	Renderer(Renderer const&) = delete;
	void operator=(Renderer const&) = delete;

	static Renderer& Instance()
	{
		static Renderer instance;
		return instance;
	}

	void RenderScene(Scene& scene, float time);

private:
	Renderer()
		:	m_MultisampledFramebuffer(CreateScope<Framebuffer>(1920, 1080, Util::GlobalConfig::MSAASamples)),
			m_Framebuffer(CreateScope<Framebuffer>(1920, 1080)),
			m_FullscreenShader(CreateScope<Shader>("assets/shaders/fullscreen.vert", "assets/shaders/fullscreen.frag"))
	{
		float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
		};
		glGenVertexArrays(1, &m_QuadVertexArray);
		glGenBuffers(1, &m_QuadVertexBuffer);
		glBindVertexArray(m_QuadVertexArray);
		glBindBuffer(GL_ARRAY_BUFFER, m_QuadVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glBindVertexArray(0);
	}

	std::unordered_map<Ref<Chunk>, std::vector<Ref<RenderTile>>, Util::HashChunk, Util::ChunkComparator> m_ChunkRenderTileMap;
	Scope<Framebuffer> m_MultisampledFramebuffer;
	Scope<Framebuffer> m_Framebuffer;
	Scope<Shader> m_FullscreenShader;
	unsigned int m_QuadVertexArray, m_QuadVertexBuffer;

	void createRenderTiles(Ref<Chunk> chunk, Ref<World> world);
	void drawTerrain(Scene& scene, float time);
	void drawSkybox(Scene& scene);

};