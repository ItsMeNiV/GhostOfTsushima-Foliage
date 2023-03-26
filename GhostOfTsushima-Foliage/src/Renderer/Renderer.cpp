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
    uint32_t chunkCount = scene.World->GetChunks().size();
    uint32_t renderTileCount = scene.World->GetChunks().size() * pow(Util::GlobalConfig::RenderTilesPerChunkSide, 2);

    m_FrustumCullTerrainComputeShader->Use();
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ComputeBufferTerrain);
    int ssbo_binding = 0;
    m_FrustumCullTerrainComputeShader->SetShaderStorageBlockBinding(0, ssbo_binding);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, m_ComputeBufferTerrain);
    if (initialRender)
    {
        glBufferStorage(GL_SHADER_STORAGE_BUFFER, (chunkCount + renderTileCount) * sizeof(uint32_t), 0, GL_MAP_READ_BIT);
        m_ChunksToRenderArray = new uint32_t[chunkCount];
        m_TilesToRenderArray = new uint32_t[renderTileCount];
        initialRender = false;
    }

    m_FrustumCullTerrainComputeShader->SetInt("chunkSize", Util::GlobalConfig::ChunkSize);
    m_FrustumCullTerrainComputeShader->SetInt("renderTileSize", Util::GlobalConfig::RenderTileSize);
    m_FrustumCullTerrainComputeShader->SetFloat("cameraWidth", scene.Camera->GetCameraWidth());
    m_FrustumCullTerrainComputeShader->SetFloat("cameraHeight", scene.Camera->GetCameraHeight());
    m_FrustumCullTerrainComputeShader->SetVec3("cameraFront", scene.Camera->GetCameraFront());
    m_FrustumCullTerrainComputeShader->SetVec3("cameraRight", scene.Camera->GetCameraRight());
    m_FrustumCullTerrainComputeShader->SetVec3("cameraUp", scene.Camera->GetCameraUp());
    m_FrustumCullTerrainComputeShader->SetVec3("cameraPos", scene.Camera->GetPosition());

    glDispatchCompute(sqrt(chunkCount), 1, sqrt(chunkCount));
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    uint32_t* p = (uint32_t*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
    memcpy(m_ChunksToRenderArray, p, chunkCount * sizeof(uint32_t));
    memcpy(m_TilesToRenderArray, &p[chunkCount], renderTileCount * sizeof(uint32_t));
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

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
    for (auto& chunkPosPair : scene.World->GetChunks())
    {
        auto chunk = chunkPosPair.second;

        //Check if chunk is on camera frustum
        int index = chunk->GetPosition().y* scene.World->GetWidth() + chunk->GetPosition().x;
        if (!m_ChunksToRenderArray[index])
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
    }

    //Grass-System
    glDisable(GL_CULL_FACE);
    for (auto& chunkPosPair : scene.World->GetChunks())
    {
        auto chunk = chunkPosPair.second;

        //Check if chunk is on camera frustum
        int chunkIndex = chunk->GetPosition().y * scene.World->GetWidth() + chunk->GetPosition().x;
        if (!m_ChunksToRenderArray[chunkIndex])
            continue;

        for (auto renderTile : m_ChunkRenderTileMap.at(chunk))
        {
            int tileIndex = (chunkIndex * pow(Util::GlobalConfig::RenderTilesPerChunkSide, 2)) + (renderTile->GetPosition().y * Util::GlobalConfig::RenderTilesPerChunkSide + renderTile->GetPosition().x);
            if (!m_TilesToRenderArray[tileIndex])
                continue;

            GrassSystem::Instance().DrawRenderTile(renderTile, scene.Camera, time);
        }
    }
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
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
