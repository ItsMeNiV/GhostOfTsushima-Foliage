#pragma once
#include "pch.h"
#include "World/World.h"
#include "Renderer/GrassMesh.h"

struct Scene
{
	Ref<World> World;
	Ref<Camera> Camera;
	Ref<Shader> TerrainShader;
	Ref<Shader> SkyboxShader;

	Ref<Shader> GrassbladeShader;
	Ref<GrassMesh> MyGrassMesh;
};