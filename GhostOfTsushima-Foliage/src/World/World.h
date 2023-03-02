#pragma once
#include "Util/Util.h"
#include "Util/Texture.h"
#include "Chunk.h"

class World
{
public:
	World(std::string&& name, uint32_t width, uint32_t height, Ref<Texture> heightmapTexture);

private:
	std::string m_Name;
	uint32_t m_Width;
	uint32_t m_Height;
	Ref<Texture> m_HeightmapTexture;
	std::unordered_map<glm::ivec2, Ref<Chunk>, Util::HashVec2> m_Chunks;
};