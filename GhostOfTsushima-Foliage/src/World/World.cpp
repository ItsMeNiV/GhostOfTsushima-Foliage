#include "pch.h"
#include "World.h"

World::World(std::string&& name, uint32_t width, uint32_t height, Ref<Texture> heightmapTexture)
	: m_Name(name), m_Width(width), m_Height(height), m_HeightmapTexture(heightmapTexture)
{
	for (uint32_t x = 0; x < width; x++)
	{
		for (uint32_t y = 0; y < height; y++)
		{
			glm::ivec2 position = glm::ivec2(x, y);
			m_Chunks[position] = CreateRef<Chunk>(position);
		}
	}
}
