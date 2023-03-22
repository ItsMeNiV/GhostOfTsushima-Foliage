#include "pch.h"
#include "GrassSystem.h"
#include "Util/Util.h"

GrassSystem::GrassSystem()
    : m_ComputeShader(CreateScope<Shader>("assets/shaders/grass_system.comp")),
	m_GrassbladeShader(CreateScope<Shader>("assets/shaders/grass.vert", "assets/shaders/grass.frag")),
	m_GrassbladeMesh(CreateScope<GrassMesh>())
{
    m_GrassbladeMesh->BindVertexArray();
	glGenBuffers(1, &m_GrassBladeDataBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_GrassBladeDataBuffer);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(GrassBlade), (void*)0);
    glEnableVertexAttribArray(4);
	glVertexAttribIPointer(4, 1, GL_UNSIGNED_INT, sizeof(GrassBlade), (void*)(offsetof(GrassBlade, Hash)));
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
	m_GrassbladeMesh->Unbind();
}

void GrassSystem::DrawRenderTile(Ref<RenderTile> renderTile, Ref<Camera> camera, float time)
{
    m_GrassbladeShader->Use();
    // Buffer Matrices
    m_GrassbladeMesh->BindVertexArray();
    glBindBuffer(GL_ARRAY_BUFFER, m_GrassBladeDataBuffer);
    glBufferData(GL_ARRAY_BUFFER, renderTile->GetGrassData()->BladeCount * sizeof(GrassBlade), renderTile->GetGrassData()->GrassBlades, GL_DYNAMIC_DRAW);
    m_GrassbladeMesh->Unbind();

    // Set uniforms and draw
    glm::mat4 view = camera->GetView();
    glm::mat4 projection = camera->GetProjection();
    glm::vec3 cameraPos = camera->GetPosition();
    m_GrassbladeShader->SetMat4("view", view);
    m_GrassbladeShader->SetMat4("projection", projection);
    m_GrassbladeShader->SetVec3("cameraPos", cameraPos);
    m_GrassbladeShader->SetMat4("renderTileModel", renderTile->GetModelMatrix());
    m_GrassbladeMesh->GetGrassbladeDiffuseTexture()->ActivateForSlot(0);
    m_GrassbladeShader->SetTexture("grassbladeTexture", 0);
    m_GrassbladeShader->SetFloat("time", time);
    m_GrassbladeMesh->BindVertexArray();
    glDrawElementsInstanced(GL_TRIANGLES, 15, GL_UNSIGNED_INT, 0, renderTile->GetGrassData()->BladeCount);
    m_GrassbladeMesh->Unbind();
}

Ref<GrassData> GrassSystem::GenerateGrassData(RenderTile& renderTile, Ref<World> world)
{
    Ref<GrassData> data = CreateRef<GrassData>();
    data->BladeCount = pow(Util::GlobalConfig::GrassBladesPerRenderTileSide, 2);

    m_ComputeShader->Use();
    unsigned int ssbo;
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    int ssbo_binding = 0;
    m_ComputeShader->SetShaderStorageBlockBinding(0, ssbo_binding);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, ssbo);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, data->BladeCount * sizeof(GrassBlade), 0, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);

    m_ComputeShader->SetVec2("chunkPos", renderTile.GetParentChunk()->GetPosition());
    m_ComputeShader->SetVec2("renderTilePos", renderTile.GetPosition());
    m_ComputeShader->SetInt("worldSize", world->GetWidth());
    m_ComputeShader->SetInt("chunkSize", Util::GlobalConfig::ChunkSize);
    m_ComputeShader->SetInt("renderTileSize", Util::GlobalConfig::RenderTileSize);
    m_ComputeShader->SetTexture("terrainHeightmapTexture", 0);
    world->GetHeightmapTexture()->ActivateForSlot(0);
    m_ComputeShader->SetTexture("terrainDiffuseTexture", 1);
    world->GetDiffuseTexture()->ActivateForSlot(1);

    glDispatchCompute(Util::GlobalConfig::GrassBladesPerRenderTileSide, 1, Util::GlobalConfig::GrassBladesPerRenderTileSide);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    std::vector<GrassBlade> tempBlades(data->BladeCount);
    GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);
    memcpy(tempBlades.data(), p, data->BladeCount * sizeof(GrassBlade));
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    glDeleteBuffers(1, &ssbo);

    tempBlades.erase(
        std::remove_if(tempBlades.begin(), tempBlades.end(),
            [](GrassBlade b) { return b.Position.y == -99.0f; }),
        tempBlades.end());
    data->BladeCount = tempBlades.size();
    data->GrassBlades = new GrassBlade[data->BladeCount];
    memcpy(data->GrassBlades, tempBlades.data(), data->BladeCount * sizeof(GrassBlade));

	return data;
}
