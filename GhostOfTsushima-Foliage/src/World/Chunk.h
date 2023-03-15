#pragma once
#include "Renderer/TerrainMesh.h"

class Chunk
{
public:
	Chunk(glm::ivec2 position, uint32_t worldWidth);

	const Ref<TerrainMesh>& GetMesh() const { return m_Mesh; }
	const glm::mat4& GetModelMatrix() const { return m_ModelMatrix; }
	const glm::ivec2& GetPosition() const { return m_Position; }

private:
	glm::ivec2 m_Position;
	//uint32_t m_ElevationLevel;
	glm::mat4 m_ModelMatrix;
	Ref<TerrainMesh> m_Mesh;

};