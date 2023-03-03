#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(uint32_t width)
{
	glGenVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);

	glGenBuffers(1, &m_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glGenBuffers(1, &m_IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0); // 2 Floats (Position X, Z)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, UVCoord)); // 2 Floats (UV-Coordinate HeightmapTexture)
	glEnableVertexAttribArray(1);

	generateMesh(width);

	Unbind();
}

void Mesh::generateMesh(uint32_t width)
{
	std::vector<VertexData> vertices;
	std::vector<uint32_t> indices;

	uint32_t chunkResolution = 2; // Vertices per "Unit" (width is in Units)
	uint32_t vertsPerSide = width * chunkResolution;
	uint32_t vertexCount = pow(vertsPerSide, 2);
	float distanceOfPoints = float(width) / vertsPerSide;
	float distanceOfTexturePoints = 1.0f / vertsPerSide;
	vertices.resize(vertexCount);


	float zPos = 0.0f;
	glm::vec2 textureCoords = { 0.0f, 0.0f };
	for (uint32_t z = 0; z < vertsPerSide; z++)
	{
		float xPos = 0.0f;
		textureCoords.x = 0.0f;
		for (uint32_t x = 0; x < vertsPerSide; x++)
		{
			VertexData vert = { {xPos, zPos}, textureCoords };
			vertices[(z * vertsPerSide) + x] = vert;
			xPos += distanceOfPoints;
			textureCoords.x += distanceOfTexturePoints;
		}
		zPos += distanceOfPoints;
		textureCoords.y += distanceOfTexturePoints;
	}

	uint32_t triangleCount = pow(vertsPerSide - 1, 2) * 2;
	uint32_t indexCount = (3 * triangleCount) + 1;
	indices.resize(indexCount);
	uint32_t indexCounter = 0;
	for (uint32_t i = 0; i < vertexCount - width; i++)
	{
		if ((i + 1) % width == 0)
			continue;
		//First triangle in cell
		indices[indexCounter] = i;
		indexCounter++;
		indices[indexCounter] = i + 1;
		indexCounter++;
		indices[indexCounter] = i + width;
		indexCounter++;

		//Second triangle in cell
		indices[indexCounter] = i + 1;
		indexCounter++;
		indices[indexCounter] = i + 1 + width;
		indexCounter++;
		indices[indexCounter] = i + width;
		indexCounter++;
	}
}
