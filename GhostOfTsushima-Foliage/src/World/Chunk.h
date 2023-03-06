#pragma once
#include "Renderer/TerrainMesh.h"

class Chunk
{
public:
	static constexpr uint32_t ChunkSize = 32;

	Chunk(glm::ivec2 position, uint32_t worldWidth);

	Ref<TerrainMesh>& GetMesh() { return m_Mesh; }
	glm::mat4& GetModelMatrix() { return m_ModelMatrix; }
	glm::ivec2& GetPosition() { return m_Position; }

private:
	glm::ivec2 m_Position;
	glm::mat4 m_ModelMatrix;
	Ref<TerrainMesh> m_Mesh;

};