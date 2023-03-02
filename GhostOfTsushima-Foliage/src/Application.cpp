#include "pch.h"
#include "Application.h"

Application::Application()
    : m_Camera(CreateRef<Camera>(glm::vec3(0.0f, 0.0f, 3.0f), m_WindowWidth, m_WindowHeight)),
      m_CameraController(CreateRef<CameraControllerFirstPerson>(m_Camera.get(), 3.0f, 0.1f))
{
    initOpenGLWithGLFW(m_WindowTitle, m_WindowWidth, m_WindowHeight);
    glEnable(GL_DEPTH_TEST);
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

        processInput(window, deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Application::loadWorld(uint32_t worldNumber)
{
    switch (worldNumber)
    {
    case 0:
        m_World = CreateRef<World>("World 0", 1024, 1024, CreateRef<Texture>("assets/textures/heightmaps/HillHeightMap.png"));
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
        m_CameraController->SetSpeed(8.0f);
        m_Sprinting = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && m_Sprinting)
    {
        m_CameraController->SetSpeed(3.0f);
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