#include "pch.h"
#include "RenderTile.h"
#include "GrassSystem.h"
#include "Util/Util.h"

RenderTile::RenderTile(Ref<Chunk> parentChunk, glm::ivec2 position)
	: m_ParentChunk(parentChunk), m_Position(position), m_ModelMatrix(1.0f)
{
	m_GrassData = GrassSystem::Instance().GenerateGrassData(*this);

	m_ModelMatrix = m_ParentChunk->GetModelMatrix() * glm::translate(m_ModelMatrix, glm::vec3(Util::GlobalConfig::RenderTileSize * position.x, 0.0f, Util::GlobalConfig::RenderTileSize * position.y));
}
