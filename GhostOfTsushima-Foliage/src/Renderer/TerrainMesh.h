#pragma once

struct VertexData
{
	glm::vec2 Position;
	glm::vec2 UVCoord;
};

class TerrainMesh
{
public:
	TerrainMesh(uint32_t width, glm::vec2 textureCoordStartingPoint, glm::vec2 textureCoordinateEndPoint);

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

	uint32_t GetIndexCount() { return m_IndexCount; }

private:
	unsigned int m_VertexArray;
	unsigned int m_VertexBuffer;
	unsigned int m_IndexBuffer;
	uint32_t m_IndexCount;

	void generateMesh(uint32_t width, glm::vec2 textureCoordStartingPoint, glm::vec2 textureCoordinateEndPoint);
};