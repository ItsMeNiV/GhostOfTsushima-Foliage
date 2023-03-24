#pragma once
#include "RenderTile.h"
#include "GrassMesh.h"
#include "World/World.h"

struct GrassBlade
{
	glm::vec3 Position;
	float Height;
	GLuint Hash;
	float __padding[3];
};

struct GrassData
{
	uint32_t BladeCount;
	GrassBlade* GrassBlades;

	~GrassData()
	{
		if(GrassBlades)
			delete[] GrassBlades;
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
	Ref<GrassData> GenerateGrassData(RenderTile& renderTile, Ref<World> world);

private:
	GrassSystem();

	Scope<Shader> m_ComputeShader;
	Scope<Shader> m_GrassbladeShader;
	Scope<GrassMesh> m_GrassbladeMesh;
	unsigned int m_GrassBladeDataBuffer;
};