#pragma once

struct VertexData
{
	glm::vec2 Position;
	glm::vec2 UVCoord;
};

class Mesh
{
public:
	Mesh(uint32_t width);

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

private:
	unsigned int m_VertexArray;
	unsigned int m_VertexBuffer;
	unsigned int m_IndexBuffer;

	void generateMesh(uint32_t width);
};