#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "Engine.h"
#include "VulkanRenderer.h"
#include "nlohmann/json.hpp"
#include "Globals.h"

#include <vector>

GLFWwindow* window;
VulkanRenderer vulkanRenderer;

int Engine::start()
{
	// Read configuration
	std::ifstream jsonfs("config.json");
	nlohmann::json config = nlohmann::json::parse(jsonfs);

	// Create Window
	initWindow("Leap Of Faith", config["width"], config["height"]);

	// Initialize global variablers
	initGlobals();

	// Create renderer instance
	if (vulkanRenderer.init(window) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	float angle = 0.0f;
	float deltaTime = 0.0f;
	float lastTime = 0.0f;

	int helicopter = vulkanRenderer.createMeshModel(config["model"]);

	// Loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		float now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		angle += 10.0 * deltaTime;
		if (angle > 360.0f) { angle -= 360.0f; }

		glm::mat4 testMat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		testMat = glm::rotate(testMat, glm::radians(-90.f), glm::vec3(1.0f, 0.0f, 0.0f));
		vulkanRenderer.updateModel(helicopter, testMat);

		vulkanRenderer.draw();
	}

	vulkanRenderer.cleanup();
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Engine::initWindow(const std::string& wName, const int width, const int height)
{
	// Initialize GLFW
	glfwInit();

	// Set GLFW to NOT work with OpenGL
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, wName.c_str(), nullptr, nullptr);
}

void Engine::initGlobals()
{
	Globals::vkContext = new Globals::VkContext();
}
