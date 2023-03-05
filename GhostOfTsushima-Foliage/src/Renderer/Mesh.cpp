#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(uint32_t width, glm::vec2 textureCoordStartingPoint, glm::vec2 textureCoordinateEndPoint)
{
	glGenVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);

	glGenBuffers(1, &m_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glGenBuffers(1, &m_IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

	generateMesh(width, textureCoordStartingPoint, textureCoordinateEndPoint);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0); // 2 Floats (Position X, Z)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(2 * sizeof(float))); // 2 Floats (UV-Coordinate HeightmapTexture)
	glEnableVertexAttribArray(1);

	Unbind();
}

void Mesh::generateMesh(uint32_t width, glm::vec2 textureCoordStartingPoint, glm::vec2 textureCoordinateEndPoint)
{
	std::vector<VertexData> vertices;
	std::vector<uint32_t> indices;

	uint32_t chunkResolution = 2; // Vertices per "Unit" (width is in Units)
	uint32_t vertsPerSide = width * chunkResolution;
	uint32_t vertexCount = pow(vertsPerSide, 2);
	float distanceOfPoints = float(width) / (vertsPerSide-1);
	glm::vec2 distanceOfTexturePoints = (textureCoordinateEndPoint - textureCoordStartingPoint) / float(vertsPerSide-1);
	vertices.resize(vertexCount);


	float zPos = 0.0f;
	glm::vec2 textureCoords = textureCoordStartingPoint;
	for (uint32_t z = 0; z < vertsPerSide; z++)
	{
		float xPos = 0.0f;
		textureCoords.x = textureCoordStartingPoint.x;
		for (uint32_t x = 0; x < vertsPerSide; x++)
		{
			VertexData vert = { {xPos, zPos}, textureCoords };
			vertices[(z * vertsPerSide) + x] = vert;
			xPos += distanceOfPoints;
			textureCoords.x += distanceOfTexturePoints.x;
		}
		zPos += distanceOfPoints;
		textureCoords.y += distanceOfTexturePoints.y;
	}

	uint32_t triangleCount = pow(vertsPerSide - 1, 2) * 2;
	m_IndexCount = (3 * triangleCount) + 1;
	indices.resize(m_IndexCount);
	uint32_t indexCounter = 0;
	for (uint32_t i = 0; i < vertexCount - vertsPerSide; i++)
	{
		if ((i + 1) % vertsPerSide == 0)
			continue;
		//First triangle in cell
		indices[indexCounter] = i;
		indexCounter++;
		indices[indexCounter] = i + 1;
		indexCounter++;
		indices[indexCounter] = i + vertsPerSide;
		indexCounter++;

		//Second triangle in cell
		indices[indexCounter] = i + 1;
		indexCounter++;
		indices[indexCounter] = i + 1 + vertsPerSide;
		indexCounter++;
		indices[indexCounter] = i + vertsPerSide;
		indexCounter++;
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m_IndexCount, &indices[0], GL_STATIC_DRAW);
}
