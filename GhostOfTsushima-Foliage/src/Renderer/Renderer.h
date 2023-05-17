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
	void DrawLine(glm::vec3 position, glm::vec3 direction, glm::vec3 color, Scene& scene);

private:
	Renderer()
		: m_MultisampledFramebuffer(CreateScope<Framebuffer>(1920, 1080, Util::GlobalConfig::MSAASamples)),
		m_Framebuffer(CreateScope<Framebuffer>(1920, 1080)),
		m_FullscreenShader(CreateScope<Shader>("assets/shaders/fullscreen.vert", "assets/shaders/fullscreen.frag")),
		m_FrustumCullTerrainComputeShader(CreateScope<Shader>("assets/shaders/frustumcull_terrain.comp")),
		m_LineDebugShader(CreateScope<Shader>("assets/shaders/linedebug.vert", "assets/shaders/linedebug.frag")),
		m_ChunksToRenderArray(nullptr),
		m_TilesToRenderArray(nullptr),
		initialRender(true)
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

		glGenBuffers(1, &m_ComputeBufferTerrain);
	}

	~Renderer()
	{
		glDeleteBuffers(1, &m_QuadVertexBuffer);
		glDeleteBuffers(1, &m_ComputeBufferTerrain);
		glDeleteVertexArrays(1, &m_QuadVertexArray);
		delete[] m_ChunksToRenderArray;
		delete[] m_TilesToRenderArray;
	}

	std::unordered_map<Ref<Chunk>, std::vector<Ref<RenderTile>>, Util::HashChunk, Util::ChunkComparator> m_ChunkRenderTileMap;
	Scope<Framebuffer> m_MultisampledFramebuffer;
	Scope<Framebuffer> m_Framebuffer;
	Scope<Shader> m_FullscreenShader;
	Scope<Shader> m_FrustumCullTerrainComputeShader;
	Scope<Shader> m_LineDebugShader;
	unsigned int m_QuadVertexArray, m_QuadVertexBuffer;
	unsigned int m_ComputeBufferTerrain;
	uint32_t* m_ChunksToRenderArray;
	uint32_t* m_TilesToRenderArray;
	bool initialRender;

	void createRenderTiles(Ref<Chunk> chunk, Ref<World> world);
	void drawTerrain(Scene& scene, float time);
	void drawSkybox(Scene& scene);

};