#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 800;
const GLint HIGHT = 600;

int main()
{
	///* GLFW initialization *///
	if (!glfwInit())
	{
		printf("GLFW initialization failed");
		glfwTerminate();
		return 1;
	}

	// Set OpenGL version to 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Core profile = No backward compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Setup GLFW window properties
	GLFWwindow *MainWindow = glfwCreateWindow(WIDTH,
											HIGHT,
											"App window",
											nullptr,
											nullptr);

	if (!MainWindow)
	{
		printf ("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get buffer size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(MainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLFW to use (here is the main window)
	glfwMakeContextCurrent(MainWindow);


	///* GLEW initialization *///
	// Allow experimental (modern) extensions to use
	glewExperimental = GL_TRUE;

	if(glewInit() != GLEW_OK)
	{
		printf ("GLEW initialization failed!");
		glfwDestroyWindow(MainWindow);
		glfwTerminate();

		return 1;
	}

	// Setup viewport size. It is less then window size and equal to drawing area (FrameBuffer)
	glViewport(0, 0, bufferWidth, bufferHeight);

	// Render loop

	while (!glfwWindowShouldClose(MainWindow))
	{
		// Get and handle user input events
		glfwPollEvents();

		// Clear window with black color
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(MainWindow);
	}

	return 0;
}