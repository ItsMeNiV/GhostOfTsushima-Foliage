#pragma once

enum TextureType
{
	TEXTURE_2D = GL_TEXTURE_2D,
	TEXTURE_CUBEMAP = GL_TEXTURE_CUBE_MAP
};

class Texture
{
public:
	Texture(std::string&& path, bool flipVertically = false);
	Texture(std::vector<std::string> paths, bool flipVertically = false);
	~Texture();

	void ActivateForSlot(uint32_t slot);

	unsigned int GetTextureId() { return m_TextureId; }
	unsigned int GetTextureType() { return m_TextureType; }

private:
	std::vector<std::string> m_Paths;
	unsigned int m_TextureId;
	TextureType m_TextureType;
};