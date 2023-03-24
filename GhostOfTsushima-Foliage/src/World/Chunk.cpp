#include "pch.h"
#include "Chunk.h"
#include "Util/Util.h"

Chunk::Chunk(glm::ivec2 position, uint32_t worldWidth)
	: m_Position(position), m_ModelMatrix(1.0f)
{
	float textureCoordDistancePerChunk = 1.0f / (worldWidth-1);
	glm::vec2 textureCoordStartingPoint = glm::vec2(m_Position.x * textureCoordDistancePerChunk, m_Position.y * textureCoordDistancePerChunk);
	glm::vec2 textureCoordEndingPoint = textureCoordStartingPoint + textureCoordDistancePerChunk;
	m_Mesh = CreateRef<TerrainMesh>(Util::GlobalConfig::ChunkSize, textureCoordStartingPoint, textureCoordEndingPoint);
	m_ModelMatrix = glm::translate(m_ModelMatrix, glm::vec3(Util::GlobalConfig::ChunkSize * position.x, 0.0f, Util::GlobalConfig::ChunkSize * position.y));
	glm::vec3 min(Util::GlobalConfig::ChunkSize * position.x, 0.0f, Util::GlobalConfig::ChunkSize * position.y);
	glm::vec3 max(min.x + Util::GlobalConfig::ChunkSize, 50.0f, min.z + Util::GlobalConfig::ChunkSize);
	m_BoundingBox = CreateRef<AABB>(min, max);
}
