#include "pch.h"
#include "RenderTile.h"
#include "GrassSystem.h"

RenderTile::RenderTile(Ref<Chunk> parentChunk, glm::ivec2 position)
	: m_ParentChunk(parentChunk), m_Position(position)
{
	m_GrassData = GrassSystem::Instance().GenerateGrassData(*this);
}
