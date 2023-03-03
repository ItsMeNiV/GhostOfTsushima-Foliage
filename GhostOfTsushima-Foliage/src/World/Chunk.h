#pragma once
#include "Renderer/Mesh.h"

class Chunk
{
public:
	static constexpr uint32_t ChunkSize = 64;

	Chunk(glm::ivec2 position);

	Ref<Mesh> GetMesh() { return m_Mesh; }
	glm::mat4 GetModelMatrix() { return m_ModelMatrix; }

private:
	glm::ivec2 m_Position;
	glm::mat4 m_ModelMatrix;
	Ref<Mesh> m_Mesh;

};