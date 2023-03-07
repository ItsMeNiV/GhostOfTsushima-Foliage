#pragma once
#include "Scene.h"
#include "World/World.h"
#include "Renderer/GrassMesh.h"

class Renderer
{
public:
	Renderer(Renderer const&) = delete;
	void operator=(Renderer const&) = delete;

	static Renderer& Instance()
	{
		static Renderer instance;
		return instance;
	}

	void RenderScene(Scene& scene, float time);

private:
	Renderer() {}

};