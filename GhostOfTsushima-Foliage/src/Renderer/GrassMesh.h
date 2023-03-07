#pragma once
#include "Util/Texture.h"

static float grassVerts[] = {
	//Position				//1D-Texture-Coordinate		//SwayStrength
	-0.05f, 0.0f,  0.0f,	0.0f,						0.0f,
	 0.05f, 0.0f,  0.0f,	1.0f,						0.0f,
	-0.05f, 0.5f,  0.0f,	0.0f,						0.3f,
	 0.05f, 0.5f,  0.0f,	1.0f,						0.3f,
	-0.05f, 0.8f,  0.0f,	0.0f,						0.6f,
	 0.05f, 0.8f,  0.0f,	1.0f,						0.6f,
	 0.0f,  1.0f,  0.0f,	0.5f,						1.0f
};

static uint32_t grassIndices[] = {
	0, 1, 2,
	1, 3, 2,
	2, 3, 4,
	3, 5, 4,
	4, 5, 6
};

class GrassMesh
{
public:
	GrassMesh();

	inline void Bind()
	{
		glBindVertexArray(m_VertexArray);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	}
	inline void Unbind()
	{
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	Ref<Texture> GetGrassbladeDiffuseTexture() { return m_GrassbladeDiffuseTexture; }

private:
	unsigned int m_VertexArray;
	unsigned int m_VertexBuffer;
	unsigned int m_IndexBuffer;
	Ref<Texture> m_GrassbladeDiffuseTexture;
};