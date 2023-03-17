#pragma once
#include "Renderer/TerrainMesh.h"
#include "World/Chunk.h"

struct GrassData;

class RenderTile
{
public:
	RenderTile(Ref<Chunk> parentChunk, glm::ivec2 position);

	Ref<GrassData> GetGrassData() { return m_GrassData; }
	glm::mat4& GetModelMatrix() { return m_ModelMatrix; }

private:
	Ref<Chunk> m_ParentChunk;
	glm::ivec2 m_Position;
	Ref<GrassData> m_GrassData;
	glm::mat4 m_ModelMatrix;
};