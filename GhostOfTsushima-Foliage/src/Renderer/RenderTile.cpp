#include "pch.h"
#include "RenderTile.h"
#include "GrassSystem.h"
#include "Util/Util.h"

RenderTile::RenderTile(Ref<Chunk> parentChunk, glm::ivec2 position, Ref<World> world)
	: m_ParentChunk(parentChunk), m_Position(position), m_ModelMatrix(1.0f)
{
	m_GrassData = GrassSystem::Instance().GenerateGrassData(*this, world);

	m_ModelMatrix = m_ParentChunk->GetModelMatrix() * glm::translate(m_ModelMatrix, glm::vec3(Util::GlobalConfig::RenderTileSize * position.x, 0.0f, Util::GlobalConfig::RenderTileSize * position.y));

	glm::vec2 parentChunkPos(m_ParentChunk->GetPosition().x * Util::GlobalConfig::ChunkSize, m_ParentChunk->GetPosition().y * Util::GlobalConfig::ChunkSize);
	glm::vec3 min(parentChunkPos.x + Util::GlobalConfig::RenderTileSize * position.x, 0.0f, parentChunkPos.y + Util::GlobalConfig::RenderTileSize * position.y);
	glm::vec3 max(min.x + Util::GlobalConfig::RenderTileSize, 50.0f, min.z + Util::GlobalConfig::RenderTileSize);
	m_BoundingBox = CreateRef<AABB>(min, max);
}
