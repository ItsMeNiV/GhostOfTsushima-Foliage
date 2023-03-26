#pragma once
#include "Renderer/TerrainMesh.h"
#include "World/Chunk.h"
#include "World/World.h"

struct GrassData;

class RenderTile
{
public:
	RenderTile(Ref<Chunk> parentChunk, glm::ivec2 position, Ref<World> world);

	glm::mat4& GetModelMatrix() { return m_ModelMatrix; }
	glm::ivec2 GetPosition() { return m_Position; }
	Ref<Chunk> GetParentChunk() { return m_ParentChunk; }
	unsigned int GetGrassInstanceBuffer() { return m_GrassInstanceBuffer; }
	uint32_t GetGrassBladeCount() { return m_GrassBladeCount; }
	void SetGrassBladeCount(uint32_t count) { m_GrassBladeCount = count; }

private:
	Ref<Chunk> m_ParentChunk;
	glm::ivec2 m_Position;
	glm::mat4 m_ModelMatrix;

	uint32_t m_GrassBladeCount;
	unsigned int m_GrassInstanceBuffer;
};