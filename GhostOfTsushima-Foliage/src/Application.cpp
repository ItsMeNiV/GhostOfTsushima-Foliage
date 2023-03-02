#include "OpenGLStarter.h"

int main()
{
	initOpenGLWithGLFW("Ghost of Tsushima Foliage Rendering", 1600, 900);

    mainLoop
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}