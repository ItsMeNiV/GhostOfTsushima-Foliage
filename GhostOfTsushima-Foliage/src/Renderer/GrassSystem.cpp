#include "pch.h"
#include "GrassSystem.h"
#include "Util/Util.h"

GrassSystem::GrassSystem()
    : m_ComputeShader(CreateScope<Shader>("assets/shaders/grass_system.comp")),
	m_GrassbladeShader(CreateScope<Shader>("assets/shaders/grass.vert", "assets/shaders/grass.frag")),
	m_GrassbladeMesh(CreateScope<GrassMesh>())
{
    uint32_t bladeCount = pow(Util::GlobalConfig::GrassBladesPerRenderTileSide, 2);
    glm::mat4* dummyData = new glm::mat4[bladeCount];
    std::fill_n(dummyData, bladeCount, glm::mat4(1.0f));
    m_GrassbladeMesh->BindVertexArray();
	glGenBuffers(1, &m_GrassBladeMatricesBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_GrassBladeMatricesBuffer);
    glBufferData(GL_ARRAY_BUFFER, bladeCount * sizeof(glm::mat4), &dummyData[0], GL_DYNAMIC_DRAW);
	size_t vec4Size = sizeof(glm::vec4);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	m_GrassbladeMesh->Unbind();
}

void GrassSystem::DrawRenderTile(Ref<RenderTile> renderTile, Ref<Camera> camera, float time)
{
    m_GrassbladeShader->Use();
    // Buffer Matrices
    m_GrassbladeMesh->BindVertexArray();
    glBindBuffer(GL_ARRAY_BUFFER, m_GrassBladeMatricesBuffer);
    glBufferData(GL_ARRAY_BUFFER, renderTile->GetGrassData()->BladeCount * sizeof(glm::mat4), renderTile->GetModelMatrices(), GL_DYNAMIC_DRAW);
    m_GrassbladeMesh->Unbind();

    // Set uniforms and draw
    glm::mat4 viewProjection = camera->GetViewProjection();
    m_GrassbladeShader->SetMat4("viewProjection", viewProjection);
    m_GrassbladeMesh->GetGrassbladeDiffuseTexture()->ActivateForSlot(0);
    m_GrassbladeShader->SetTexture("grassbladeTexture", 0);
    m_GrassbladeShader->SetFloat("time", time);
    m_GrassbladeMesh->BindVertexArray();
    glDrawElementsInstanced(GL_TRIANGLES, 15, GL_UNSIGNED_INT, 0, renderTile->GetGrassData()->BladeCount);
    m_GrassbladeMesh->Unbind();
}

Ref<GrassData> GrassSystem::GenerateGrassData(RenderTile& renderTile)
{
    Ref<GrassData> data = CreateRef<GrassData>();
    data->BladeCount = pow(Util::GlobalConfig::GrassBladesPerRenderTileSide, 2);
    data->Hashes = new uint8_t[data->BladeCount];
    data->Positions = new glm::vec3[data->BladeCount];

    std::fill_n(data->Hashes, data->BladeCount, 1);
    uint32_t i = 0;
    float distanceBetweenGrassBlades = float(Util::GlobalConfig::RenderTileSize) / float(Util::GlobalConfig::GrassBladesPerRenderTileSide);
    glm::vec3 position(0.0f, 4.0f, 0.0f);
    float LO = -0.2f;
    float HI = 0.2f;
    for (uint32_t z = 0; z < Util::GlobalConfig::GrassBladesPerRenderTileSide; z++)
    {
        position.x = 0.0f;
        for (uint32_t x = 0; x < Util::GlobalConfig::GrassBladesPerRenderTileSide; x++)
        {
            float jitterX = LO + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (HI - LO));
            float jitterZ = LO + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (HI - LO));
            data->Positions[i] = position;
            data->Positions[i].x += jitterX;
            data->Positions[i].z += jitterZ;
            i++;
            position.x += distanceBetweenGrassBlades;
        }
        position.z += distanceBetweenGrassBlades;
    }
	return data;
}
