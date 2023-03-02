#pragma once
#include "World/World.h"

class Application
{
public:
	Application();
	~Application();

	void Run();

private:
	const char* m_WindowTitle = "Ghost of Tsushima Foliage Rendering";
	const uint32_t m_WindowWidth = 1600;
	const uint32_t m_WindowHeight = 900;

	Ref<World> m_World;

	void loadWorld(uint32_t worldNumber);
};