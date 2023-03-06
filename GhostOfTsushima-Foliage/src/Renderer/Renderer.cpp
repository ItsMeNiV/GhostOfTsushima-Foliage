#include "pch.h"
#include "Renderer.h"

void Renderer::RenderScene(Scene& scene)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    //Terrain
    scene.TerrainShader->Use();
    glm::mat4 viewProjection = scene.Camera->GetViewProjection();
    scene.TerrainShader->SetMat4("viewProjection", viewProjection);
    scene.TerrainShader->SetVec3("viewPos", scene.Camera->GetPosition());
    scene.TerrainShader->SetInt("heightmapTexture", 0);
    scene.World->GetHeightmapTexture()->ActivateForSlot(0);
    scene.TerrainShader->SetInt("normalmapTexture", 1);
    scene.World->GetNormalmapTexture()->ActivateForSlot(1);
    scene.TerrainShader->SetInt("diffuseTexture", 2);
    scene.World->GetDiffuseTexture()->ActivateForSlot(2);
    for (auto& chunk : scene.World->GetChunks())
    {
        chunk.second->GetMesh()->Bind();
        scene.TerrainShader->SetMat4("model", chunk.second->GetModelMatrix());
        scene.TerrainShader->SetVec3("color", glm::vec3(chunk.second->GetPosition().x / 5.0f, 0.0f, chunk.second->GetPosition().y / 5.0f));
        glDrawElements(GL_TRIANGLES, chunk.second->GetMesh()->GetIndexCount(), GL_UNSIGNED_INT, 0);
    }
    glDisable(GL_CULL_FACE);

    //Skybox
    glDepthFunc(GL_LEQUAL);
    scene.World->GetSkybox()->Bind();
    scene.SkyboxShader->Use();
    scene.SkyboxShader->SetMat4("projection", scene.Camera->GetProjection());
    glm::mat4 view = glm::mat4(glm::mat3(scene.Camera->GetView()));
    scene.SkyboxShader->SetMat4("view", view);
    scene.SkyboxShader->SetInt("skybox", 1);
    scene.World->GetSkybox()->GetTexture()->ActivateForSlot(1);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    scene.World->GetSkybox()->Unbind();
    glDepthFunc(GL_LESS);
}
