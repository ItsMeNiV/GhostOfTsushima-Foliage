#pragma once
#include "Renderer/TerrainMesh.h"

class Chunk
{
public:
	static constexpr uint32_t ChunkSize = 32;

	Chunk(glm::ivec2 position, uint32_t worldWidth);

	Ref<TerrainMesh>& GetMesh() { return m_Mesh; }
	glm::mat4 GetModelMatrix() { return m_ModelMatrix; }

	void Draw(Shader& shader)
	{
		shader.SetMat4("model", m_ModelMatrix);
		shader.SetVec3("color", glm::vec3(m_Position.x / 5.0f, 0.0f, m_Position.y / 5.0f));
		m_Mesh->Bind();
		glDrawElements(GL_TRIANGLES, m_Mesh->GetIndexCount(), GL_UNSIGNED_INT, 0);
	}

private:
	glm::ivec2 m_Position;
	glm::mat4 m_ModelMatrix;
	Ref<TerrainMesh> m_Mesh;

};