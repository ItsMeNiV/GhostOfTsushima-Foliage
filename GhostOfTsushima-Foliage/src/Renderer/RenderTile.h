#pragma once
#include "Renderer/TerrainMesh.h"
#include "World/Chunk.h"

struct GrassData;

class RenderTile
{
public:
	RenderTile(Ref<Chunk> parentChunk, glm::ivec2 position);

	const Ref<GrassData> const GetGrassData() { return m_GrassData; }

private:
	Ref<Chunk> m_ParentChunk;
	glm::ivec2 m_Position;
	Ref<GrassData> m_GrassData;
};