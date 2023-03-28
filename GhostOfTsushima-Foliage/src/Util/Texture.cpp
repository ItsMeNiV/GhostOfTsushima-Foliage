#include "pch.h"
#include "Texture.h"

#include "stb_image.h"

Texture::Texture(std::string&& path, bool flipVertically)
    : m_Paths({ path }), m_TextureType(TextureType::TEXTURE_2D)
{
    stbi_set_flip_vertically_on_load(flipVertically);

	glGenTextures(1, &m_TextureId);
	glBindTexture(m_TextureType, m_TextureId);

    glTexParameteri(m_TextureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(m_TextureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(m_TextureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(m_TextureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(m_Paths[0].c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        int format = nrChannels == 3 ? GL_RGB : GL_RGBA;
        glTexImage2D(m_TextureType, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(m_TextureType);
    }
    else
    {
        std::cout << "Failed to load texture at path: " << m_Paths[0] << std::endl;
    }
    stbi_image_free(data);
    glBindTexture(m_TextureType, 0);
}

Texture::Texture(std::vector<std::string> paths, bool flipVertically)
    : m_Paths(paths), m_TextureType(TextureType::TEXTURE_CUBEMAP)
{
    stbi_set_flip_vertically_on_load(flipVertically);

    glGenTextures(1, &m_TextureId);
    glBindTexture(m_TextureType, m_TextureId);

    

    int width, height, nrChannels;
    for (unsigned int i = 0; i < paths.size(); i++)
    {
        unsigned char* data = stbi_load(paths[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            int format = nrChannels == 3 ? GL_RGB : GL_RGBA;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Failed to load texture at path: " << paths[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

Texture::Texture(int width, int height, int sampleCount)
    : m_TextureType(sampleCount > 1 ? TextureType::TEXTURE_2D_MULTI : TextureType::TEXTURE_2D), m_TextureId(UINT_MAX)
{
    glGenTextures(1, &m_TextureId);
    glBindTexture(m_TextureType, m_TextureId);
    if (sampleCount > 1)
        glTexImage2DMultisample(m_TextureType, sampleCount, GL_RGBA16F, width, height, GL_TRUE);
    else
        glTexImage2D(m_TextureType, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_INT, NULL);
    glTexParameteri(m_TextureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(m_TextureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(m_TextureType, 0);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_TextureId);
}

void Texture::ActivateForSlot(uint32_t slot)
{
	assert(slot <= 32);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(m_TextureType, m_TextureId);
}
