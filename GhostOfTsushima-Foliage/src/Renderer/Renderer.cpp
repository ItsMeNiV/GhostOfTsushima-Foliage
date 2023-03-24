#include "pch.h"
#include "Renderer.h"
#include "GrassSystem.h"

void Renderer::RenderScene(Scene& scene, float time)
{
    glEnable(GL_DEPTH_TEST);

    if (m_Framebuffer->GetWidth() != scene.Camera->GetCameraWidth() || m_Framebuffer->GetHeight() != scene.Camera->GetCameraHeight())
    {
        m_MultisampledFramebuffer = CreateScope<Framebuffer>(scene.Camera->GetCameraWidth(), scene.Camera->GetCameraHeight(), Util::GlobalConfig::MSAASamples);
        m_Framebuffer = CreateScope<Framebuffer>(scene.Camera->GetCameraWidth(), scene.Camera->GetCameraHeight());
    }

    m_MultisampledFramebuffer->Bind();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    drawTerrain(scene, time);
    drawSkybox(scene);
    m_MultisampledFramebuffer->Unbind();

    m_MultisampledFramebuffer->BlitFramebuffer(m_Framebuffer->GetId());
    m_FullscreenShader->Use();
    m_FullscreenShader->SetInt("screenTexture", 0);
    m_Framebuffer->GetTextureColorBuffer()->ActivateForSlot(0);
    glBindVertexArray(m_QuadVertexArray);
    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::createRenderTiles(Ref<Chunk> chunk, Ref<World> world)
{
    std::vector<Ref<RenderTile>> renderTiles;

    for (uint32_t x = 0; x < Util::GlobalConfig::RenderTilesPerChunkSide; x++)
    {
        for (uint32_t y = 0; y < Util::GlobalConfig::RenderTilesPerChunkSide; y++)
        {
            glm::ivec2 position = glm::ivec2(x, y);
            renderTiles.push_back(CreateRef<RenderTile>(chunk, position, world));
        }
    }

    m_ChunkRenderTileMap.insert({ chunk, renderTiles });
}

void Renderer::drawTerrain(Scene& scene, float time)
{
    for (auto& chunkPosPair : scene.World->GetChunks())
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
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);

        auto chunk = chunkPosPair.second;

        //Check if chunk is on camera frustum
        if (!chunk->GetBoundingBox()->IsOnFrustum(scene.Camera->GetCameraFrustum()))
            continue;

        // Check if Chunk already had RenderTiles created
        if (auto foundChunk = m_ChunkRenderTileMap.find(chunk); foundChunk == m_ChunkRenderTileMap.end())
        {
            createRenderTiles(chunk, scene.World);
        }

        chunk->GetMesh()->Bind();
        glm::mat4 model = chunk->GetModelMatrix();
        scene.TerrainShader->SetMat4("model", model);
        scene.TerrainShader->SetVec3("color", glm::vec3(chunk->GetPosition().x / 5.0f, 0.0f, chunk->GetPosition().y / 5.0f));
        glDrawElements(GL_TRIANGLES, chunk->GetMesh()->GetIndexCount(), GL_UNSIGNED_INT, 0);

        //Grass-System
        glDisable(GL_CULL_FACE);
        for (auto renderTile : m_ChunkRenderTileMap.at(chunk))
        {
            if (!renderTile->GetBoundingBox()->IsOnFrustum(scene.Camera->GetCameraFrustum()))
                continue;

            GrassSystem::Instance().DrawRenderTile(renderTile, scene.Camera, time);
        }
    }

}

void Renderer::drawSkybox(Scene& scene)
{
    glDisable(GL_CULL_FACE);
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
