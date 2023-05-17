#include "pch.h"
#include "Application.h"

#include "Renderer/Renderer.h"

Application::Application()
    : m_Camera(CreateRef<Camera>(glm::vec3(0.0f, 0.0f, 0.0f), m_WindowWidth, m_WindowHeight)),
      m_CameraController(CreateRef<CameraControllerFirstPerson>(m_Camera.get(), 10.0f, 0.1f))
{
    initOpenGLWithGLFW(m_WindowTitle, m_WindowWidth, m_WindowHeight);

    //glEnable(GL_DEBUG_OUTPUT);
    //glDebugMessageCallback(MessageCallback, 0);

    glfwSwapInterval(0);
    disableCursor();
    glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));

    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos) {
        Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
        app->mouseCallback(window, xPos, yPos);
    });
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
        app->keyCallback(window, key, scancode, action, mods);
    });
    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
        Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
        app->GetCamera()->UpdateWindowSize(width, height);
        });

    loadWorld(0);
    m_Scene.Camera = m_Camera;
    m_Scene.World = m_World;
    m_Scene.TerrainShader = CreateRef<Shader>("assets/shaders/terrain.glsl", ShaderType::VERTEX_AND_FRAGMENT);
    m_Scene.SkyboxShader = CreateRef<Shader>("assets/shaders/skybox.glsl", ShaderType::VERTEX_AND_FRAGMENT);

    m_Scene.Offsets = new glm::vec2[1000];
    float offset = 0.1f;
    uint32_t x = 0;
    uint32_t z = 0;
    for (uint32_t i = 0; i < 1000; i++)
    {
        if (x == 100)
        {
            x = 0;
            z++;
        }
        m_Scene.Offsets[i] = glm::vec2(x * offset, z * offset);
        x++;
    }
}

Application::~Application()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::Run()
{
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    mainLoop
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //std::cout << "Frametime: " << std::to_string(deltaTime) << std::endl;

        m_Camera->UpdateCameraVectors();

        processInput(window, deltaTime);

        Renderer::Instance().RenderScene(m_Scene, glfwGetTime());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Application::loadWorld(uint32_t worldNumber)
{
    std::vector<std::string> skyboxFaces = {
        "assets/textures/skybox/right.jpg",
        "assets/textures/skybox/left.jpg",
        "assets/textures/skybox/top.jpg",
        "assets/textures/skybox/bottom.jpg",
        "assets/textures/skybox/front.jpg",
        "assets/textures/skybox/back.jpg"
    };
    Ref<Skybox> skybox = CreateRef<Skybox>(CreateRef<Texture>(skyboxFaces));
    switch (worldNumber)
    {
    case 0:
        m_World = CreateRef<World>("World 0", 10, 10, 
            CreateRef<Texture>("assets/textures/heightmaps/HillHeightMap.png"), CreateRef<Texture>("assets/textures/HillNormalMap.png"),
            CreateRef<Texture>("assets/textures/HillDiffuse.png"), skybox);
        break;
    default:
        std::cout << "World with the given number " << std::to_string(worldNumber) << " not available!" << std::endl;
        break;
    }
}

void Application::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        m_IsFocused = !m_IsFocused;
        if (m_IsFocused)
            disableCursor();
        else
            enableCursor();
        m_FirstMouse = true;
    }
}

void Application::mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (m_IsFocused)
    {
        float xpos = static_cast<float>(xPos);
        float ypos = static_cast<float>(yPos);

        if (m_FirstMouse)
        {
            m_LastX = xpos;
            m_LastY = ypos;
            m_FirstMouse = false;
        }

        float xOffset = xpos - m_LastX;
        float yOffset = m_LastY - ypos;
        m_LastX = xpos;
        m_LastY = ypos;

        m_CameraController->ProcessMouseMovement(xOffset, yOffset);
    }
}

void Application::processInput(GLFWwindow* window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
        return;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_CameraController->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_CameraController->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_CameraController->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_CameraController->ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && !m_Sprinting)
    {
        m_CameraController->SetSpeed(15.0f);
        m_Sprinting = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && m_Sprinting)
    {
        m_CameraController->SetSpeed(10.0f);
        m_Sprinting = false;
    }
}

int main()
{
    Application* app = new Application();

    app->Run();

    delete app;
    return 0;
}