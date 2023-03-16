#include "pch.h"
#include "RenderTile.h"
#include "GrassSystem.h"
#include "Util/Util.h"

RenderTile::RenderTile(Ref<Chunk> parentChunk, glm::ivec2 position)
	: m_ParentChunk(parentChunk), m_Position(position), m_ModelMatrix(1.0f)
{
	m_GrassData = GrassSystem::Instance().GenerateGrassData(*this);
	uint32_t bladeCount = m_GrassData->BladeCount;
	m_ModelMatrices = new glm::mat4[bladeCount];

	m_ModelMatrix = glm::translate(m_ModelMatrix, glm::vec3(Util::GlobalConfig::RenderTileSize * position.x, 0.0f, Util::GlobalConfig::RenderTileSize * position.y));

	for (uint32_t i = 0; i < bladeCount; i++)
	{
		glm::mat4 model(1.0f);
		model = glm::translate(model, m_GrassData->Positions[i]);
		m_ModelMatrices[i] = m_ParentChunk->GetModelMatrix() * m_ModelMatrix * model;
	}
}
