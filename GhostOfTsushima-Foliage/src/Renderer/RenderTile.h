#pragma once
#include "Renderer/TerrainMesh.h"
#include "World/Chunk.h"
#include "World/World.h"

struct GrassData;

class RenderTile
{
public:
	RenderTile(Ref<Chunk> parentChunk, glm::ivec2 position, Ref<World> world);

	Ref<GrassData> GetGrassData() { return m_GrassData; }
	glm::mat4& GetModelMatrix() { return m_ModelMatrix; }
	glm::ivec2 GetPosition() { return m_Position; }
	Ref<Chunk> GetParentChunk() { return m_ParentChunk; }

private:
	Ref<Chunk> m_ParentChunk;
	glm::ivec2 m_Position;
	Ref<GrassData> m_GrassData;
	glm::mat4 m_ModelMatrix;
};