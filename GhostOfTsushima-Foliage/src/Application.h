#pragma once
#include "World/World.h"

class Application
{
public:
	Application();
	~Application();

	Ref<Camera> GetCamera() { return m_Camera; }

	void Run();

private:
	const char* m_WindowTitle = "Ghost of Tsushima Foliage Rendering";
	const uint32_t m_WindowWidth = 1600;
	const uint32_t m_WindowHeight = 900;
	bool m_Sprinting = false;
	bool m_IsFocused = true;
	bool m_FirstMouse = true;
	float m_LastX = m_WindowWidth / 2.0f;
	float m_LastY = m_WindowHeight / 2.0f;

	Ref<World> m_World;
	Ref<Camera> m_Camera;
	Ref<CameraControllerFirstPerson> m_CameraController;

	void loadWorld(uint32_t worldNumber);

	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouseCallback(GLFWwindow* window, double xPos, double yPos);
	void processInput(GLFWwindow* window, float deltaTime);
};