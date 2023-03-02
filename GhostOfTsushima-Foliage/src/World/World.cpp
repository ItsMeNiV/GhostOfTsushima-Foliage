#include "pch.h"
#include "World.h"

World::World(std::string&& name, uint32_t width, uint32_t height, Ref<Texture> heightmapTexture)
	: m_Name(name), m_Width(width), m_Height(height), m_HeightmapTexture(heightmapTexture)
{}
