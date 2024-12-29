#include "MainWindow.h"

MainWindow::MainWindow(GLint WinWidth, GLint WinHeight)
{
	Width = WinWidth;
	Height = WinHeight;

	Initialize();
}

MainWindow::~MainWindow()
{
	glfwDestroyWindow(MainWindowPtr);
	glfwTerminate();
}

int MainWindow::Initialize()
{
	///* GLFW initialization *///
	if (!glfwInit())
	{
		printf("GLFW initialization failed");
		glfwTerminate();
		return 1;
	}

	// Core profile = No backward compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Set OpenGL version to 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Setup GLFW window properties
	MainWindowPtr = glfwCreateWindow(Width,
									Height,
									"App window",
									nullptr,
									nullptr);

	if (!MainWindowPtr)
	{
		printf ("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get buffer size information
	glfwGetFramebufferSize(MainWindowPtr, &BufferWidth, &BufferHeight);

	// Set context for GLFW to use (here is the main window)
	glfwMakeContextCurrent(MainWindowPtr);

	///* GLEW initialization *///
	// Allow experimental (modern) extensions to use
	glewExperimental = GL_TRUE;

	if(glewInit() != GLEW_OK)
	{
		printf ("GLEW initialization failed!");
		glfwDestroyWindow(MainWindowPtr);
		glfwTerminate();

		return 1;
	}
	/// * END of GLEW initialization* ///

	glEnable(GL_DEPTH_TEST);
	
	// Setup viewport size. It is less then window size and equal to drawing area (FrameBuffer)
	glViewport(0, 0, BufferWidth, BufferHeight);
}

GLfloat MainWindow::GetBufferWidth() const
{
	return static_cast<float>(BufferWidth);
}

GLfloat MainWindow::GetBufferHeight() const
{
	return static_cast<float>(BufferHeight);
}

void MainWindow::SwapBuffers()
{
	glfwSwapBuffers(MainWindowPtr);
}

bool MainWindow::GetShouldClose()
{
	return glfwWindowShouldClose(MainWindowPtr);
}
