#pragma once
#include "RenderTile.h"

struct GrassData
{
	uint32_t BladeCount;
	glm::ivec2* Positions;
	uint8_t* Hashes;
};

class GrassSystem
{
public:
	GrassSystem(GrassSystem const&) = delete;
	void operator=(GrassSystem const&) = delete;

	static GrassSystem& Instance()
	{
		static GrassSystem instance;

		return instance;
	}

	void DrawRenderTile(Ref<RenderTile> renderTile);
	Ref<GrassData> GenerateGrassData(RenderTile& renderTile);

private:
	GrassSystem() : m_ComputeShader(CreateScope<Shader>("assets/shaders/grass_system.comp")) {}

	Scope<Shader> m_ComputeShader;
};