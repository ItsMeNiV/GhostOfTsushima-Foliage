#pragma once
#include "Scene.h"
#include "World/World.h"
#include "Renderer/GrassMesh.h"
#include "Renderer/RenderTile.h"
#include "Util/Util.h"

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
	Renderer() {}

	std::unordered_map<Ref<Chunk>, std::vector<Ref<RenderTile>>, Util::HashChunk, Util::ChunkComparator> m_ChunkRenderTileMap;

	void createRenderTiles(Ref<Chunk> chunk, Ref<World> world);
	void drawTerrain(Scene& scene, float time);
	void drawSkybox(Scene& scene);

};