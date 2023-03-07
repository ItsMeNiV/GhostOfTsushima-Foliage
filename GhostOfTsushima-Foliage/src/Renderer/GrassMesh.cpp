#include "pch.h"
#include "GrassMesh.h"

GrassMesh::GrassMesh()
{
	m_GrassbladeDiffuseTexture = CreateRef<Texture>("assets/textures/Grassblade.png");

	glGenVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);

	glGenBuffers(1, &m_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glGenBuffers(1, &m_IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(grassVerts), grassVerts, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grassIndices), grassIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0); // 3 Floats (Position X, Y, Z)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(3 * sizeof(float))); // 2 Floats (1D-Texture-Coordinate)
	glEnableVertexAttribArray(1);

	Unbind();
}
