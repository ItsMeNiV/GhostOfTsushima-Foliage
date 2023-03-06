#pragma once
#include "pch.h"
#include "World/World.h"

struct Scene
{
	Ref<World> World;
	Ref<Camera> Camera;
	Ref<Shader> TerrainShader;
	Ref<Shader> SkyboxShader;
};