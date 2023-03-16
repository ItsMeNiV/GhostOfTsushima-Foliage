#pragma once
#include "RenderTile.h"
#include "GrassMesh.h"

struct GrassData
{
	uint32_t BladeCount;
	glm::vec3* Positions;
	uint8_t* Hashes;

	~GrassData()
	{
		if(Positions)
			delete[] Positions;
		if(Hashes)
			delete[] Hashes;
	}
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

	void DrawRenderTile(Ref<RenderTile> renderTile, Ref<Camera> camera, float time);
	Ref<GrassData> GenerateGrassData(RenderTile& renderTile);

private:
	GrassSystem();

	Scope<Shader> m_ComputeShader;
	Scope<Shader> m_GrassbladeShader;
	Scope<GrassMesh> m_GrassbladeMesh;
	unsigned int m_GrassBladeMatricesBuffer;
};