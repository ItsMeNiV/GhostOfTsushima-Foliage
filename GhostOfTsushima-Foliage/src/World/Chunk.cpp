#include "pch.h"
#include "Chunk.h"

Chunk::Chunk(glm::ivec2 position, uint32_t worldWidth)
	: m_Position(position), m_ModelMatrix(1.0f)
{
	float textureCoordDistancePerChunk = 1.0f / (worldWidth-1);
	glm::vec2 textureCoordStartingPoint = glm::vec2(m_Position.x * textureCoordDistancePerChunk, m_Position.y * textureCoordDistancePerChunk);
	glm::vec2 textureCoordEndingPoint = textureCoordStartingPoint + textureCoordDistancePerChunk;
	m_Mesh = CreateRef<Mesh>(Chunk::ChunkSize, textureCoordStartingPoint, textureCoordEndingPoint);
	m_ModelMatrix = glm::translate(m_ModelMatrix, glm::vec3(Chunk::ChunkSize * position.x, 0.0f, Chunk::ChunkSize * position.y));
}
