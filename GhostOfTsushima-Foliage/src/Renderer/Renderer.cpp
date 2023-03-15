#include "pch.h"
#include "Renderer.h"
#include "GrassSystem.h"

void Renderer::RenderScene(Scene& scene, float time)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    drawTerrain(scene, time);
    drawSkybox(scene);
}

void Renderer::createRenderTiles(Ref<Chunk> chunk)
{
    std::vector<Ref<RenderTile>> renderTiles;

    for (uint32_t x = 0; x < Util::GlobalConfig::RenderTilesPerChunkSide; x++)
    {
        for (uint32_t y = 0; y < Util::GlobalConfig::RenderTilesPerChunkSide; y++)
        {
            glm::ivec2 position = glm::ivec2(x, y);
            renderTiles.push_back(CreateRef<RenderTile>(chunk, position));
        }
    }

    m_ChunkRenderTileMap.insert({ chunk, renderTiles });
}

void Renderer::drawTerrain(Scene& scene, float time)
{
    scene.TerrainShader->Use();
    glm::mat4 viewProjection = scene.Camera->GetViewProjection();
    scene.TerrainShader->SetMat4("viewProjection", viewProjection);
    scene.TerrainShader->SetVec3("viewPos", scene.Camera->GetPosition());
    scene.TerrainShader->SetTexture("heightmapTexture", 0);
    scene.World->GetHeightmapTexture()->ActivateForSlot(0);
    scene.TerrainShader->SetTexture("normalmapTexture", 1);
    scene.World->GetNormalmapTexture()->ActivateForSlot(1);
    scene.TerrainShader->SetTexture("diffuseTexture", 2);
    scene.World->GetDiffuseTexture()->ActivateForSlot(2);

    for (auto& chunkPosPair : scene.World->GetChunks())
    {
        auto chunk = chunkPosPair.second;

        // Check if Chunk already had RenderTiles created
        if (auto foundChunk = m_ChunkRenderTileMap.find(chunk); foundChunk == m_ChunkRenderTileMap.end())
        {
            createRenderTiles(chunk);
        }

        chunk->GetMesh()->Bind();
        glm::mat4 model = chunk->GetModelMatrix();
        scene.TerrainShader->SetMat4("model", model);
        scene.TerrainShader->SetVec3("color", glm::vec3(chunk->GetPosition().x / 5.0f, 0.0f, chunk->GetPosition().y / 5.0f));
        glDrawElements(GL_TRIANGLES, chunk->GetMesh()->GetIndexCount(), GL_UNSIGNED_INT, 0);
    }
    glDisable(GL_CULL_FACE);

    //Grass-System
    for (auto& chunkPosPair : scene.World->GetChunks())
    {
        for(auto renderTile : m_ChunkRenderTileMap.at(chunkPosPair.second))
            GrassSystem::Instance().DrawRenderTile(renderTile);
    }

    //TODO REMOVE WHEN GRASS SYSTEM IS IMPLEMENTED
    scene.GrassbladeShader->Use();
    for (unsigned int i = 0; i < 1000; i++)
    {
        scene.GrassbladeShader->SetVec2(("offsets[" + std::to_string(i) + "]"), scene.Offsets[i]);
    }
    scene.GrassbladeShader->SetMat4("viewProjection", viewProjection);
    scene.MyGrassMesh->GetGrassbladeDiffuseTexture()->ActivateForSlot(0);
    scene.GrassbladeShader->SetTexture("grassbladeTexture", 0);
    scene.GrassbladeShader->SetTexture("terrainHeightmapTexture", 1);
    scene.World->GetHeightmapTexture()->ActivateForSlot(1);
    glm::mat4 model(1.0f);
    scene.GrassbladeShader->SetMat4("model", model);
    scene.GrassbladeShader->SetFloat("time", time);
    scene.MyGrassMesh->Bind();
    glDrawElementsInstanced(GL_TRIANGLES, 15, GL_UNSIGNED_INT, 0, 1000);
}

void Renderer::drawSkybox(Scene& scene)
{
    glDepthFunc(GL_LEQUAL);
    scene.World->GetSkybox()->Bind();
    scene.SkyboxShader->Use();
    scene.SkyboxShader->SetMat4("projection", scene.Camera->GetProjection());
    glm::mat4 view = glm::mat4(glm::mat3(scene.Camera->GetView()));
    scene.SkyboxShader->SetMat4("view", view);
    scene.SkyboxShader->SetTexture("skybox", 1);
    scene.World->GetSkybox()->GetTexture()->ActivateForSlot(1);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    scene.World->GetSkybox()->Unbind();
    glDepthFunc(GL_LESS);
}
