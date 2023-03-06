#pragma once
#include "Util/Util.h"
#include "Util/Texture.h"
#include "Chunk.h"
#include "Skybox.h"

class World
{
public:
	World(std::string&& name, uint32_t width, uint32_t height, Ref<Texture> heightmapTexture, Ref<Texture> normalmapTexture, Ref<Texture> diffuseTexture,Ref<Skybox> skybox);

	std::unordered_map<glm::ivec2, Ref<Chunk>, Util::HashVec2>& GetChunks() { return m_Chunks; }

	Ref<Texture>& GetHeightmapTexture() { return m_HeightmapTexture; }
	Ref<Texture>& GetNormalmapTexture() { return m_NormalmapTexture; }
	Ref<Texture>& GetDiffuseTexture() { return m_DiffuseTexture; }
	Ref<Skybox>& GetSkybox() { return m_Skybox; }

private:
	std::string m_Name;
	uint32_t m_Width;
	uint32_t m_Height;
	Ref<Texture> m_HeightmapTexture;
	Ref<Texture> m_NormalmapTexture;
	Ref<Texture> m_DiffuseTexture;
	Ref<Skybox> m_Skybox;
	std::unordered_map<glm::ivec2, Ref<Chunk>, Util::HashVec2> m_Chunks;
};