#include "pch.h"
#include "Chunk.h"

Chunk::Chunk(glm::ivec2 position)
	: m_Position(position), m_Mesh(CreateRef<Mesh>(Chunk::ChunkSize)), m_ModelMatrix(1.0f)
{
	m_ModelMatrix = glm::translate(m_ModelMatrix, glm::vec3(Chunk::ChunkSize * position.x, 0.0f, Chunk::ChunkSize * position.y));
}
