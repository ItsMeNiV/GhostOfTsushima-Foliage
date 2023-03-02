#include "pch.h"
#include "Application.h"

Application::Application()
{
    initOpenGLWithGLFW(m_WindowTitle, m_WindowWidth, m_WindowHeight);
    loadWorld(0);
}

Application::~Application()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::Run()
{
    mainLoop
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

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

int main()
{
    Application* app = new Application();

    app->Run();

    delete app;
    return 0;
}