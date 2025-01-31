#include "Core/MainWindow.h"

#include <cstdio>
#include <format>

MainWindow::MainWindow(GLint WinWidth, GLint WinHeight)
{
	Width = WinWidth;
	Height = WinHeight;

	IsMouseMovedFirst = true;
	 
	LastX = 0.0;
	LastY = 0.0;
	OffsetX = 0.0;
	OffsetY = 0.0;

	for (auto& Key : Keys)
	{
		Key = false;
	}

	Initialize();
}

MainWindow::~MainWindow()
{
	glfwDestroyWindow(MainWindowPtr);
	glfwTerminate();

	MainWindowPtr = nullptr;
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

	// Start Input handling
	glfwSetKeyCallback(MainWindowPtr, HandleKeys);
	glfwSetCursorPosCallback(MainWindowPtr, HandleMouse);

	glfwSetInputMode(MainWindowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

#if defined (DEBUG) || defined (_DEBUG)
	// Error handling
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageCallback([](GLuint source,
		GLuint type,
		GLuint id,
		GLuint severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam) {
		printf("OpenGL Debug Message: %s\n", message);
	}, nullptr);
#endif

	// Setup viewport size. It is less then window size and equal to drawing area (FrameBuffer)
	glViewport(0, 0, BufferWidth, BufferHeight);

	glfwSetWindowUserPointer(MainWindowPtr, this);

	return 0;
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

bool* MainWindow::GetKeys()
{
	return Keys;
}

GLdouble MainWindow::GetOffsetX()
{
	const auto LastOffsetX = OffsetX;
	OffsetX = 0.0;

	return LastOffsetX;
}

GLdouble MainWindow::GetOffsetY()
{
	const auto LastOffsetY = OffsetY;
	OffsetY = 0.0;

	return LastOffsetY;
}

void MainWindow::HandleMouse(GLFWwindow* Window, double XPos, double YPos)
{
	const auto AppWindow = static_cast<MainWindow*>(glfwGetWindowUserPointer(Window));

	if (AppWindow)
	{
		if (AppWindow->IsMouseMovedFirst)
		{
			AppWindow->LastX = XPos;
			AppWindow->LastY = YPos;
			AppWindow->IsMouseMovedFirst = false;
		}

		AppWindow->OffsetX = XPos - AppWindow->LastX;
		AppWindow->OffsetY = AppWindow->LastY - YPos;

		AppWindow->LastX = XPos;
		AppWindow->LastY = YPos;

		//printf("x: %.6f, y: %.6f\n", AppWindow->OffsetX, AppWindow->OffsetY);
	}
}

void MainWindow::HandleKeys(GLFWwindow* Window, int Key, int ScanCode, int Action, int Mode)
{
	const auto AppWindow = static_cast<MainWindow*>(glfwGetWindowUserPointer(Window));

	if (AppWindow)
	{
		if (Key == GLFW_KEY_ESCAPE && Action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(Window, GL_TRUE);
		}

		if (Key >= 0 && Key < 1024)
		{
			if (Action == GLFW_PRESS)
			{
				AppWindow->Keys[Key] = true;
			}
			else if (Action == GLFW_RELEASE)
			{
				AppWindow->Keys[Key] = false;
			}

			if (Key == GLFW_KEY_UNKNOWN)
			{
				printf("Unknown key pressed with ScanCode: %d\n", ScanCode);
			}
		}
	}
}

void MainWindow::ShowFPS()
{
	static double LastTime = glfwGetTime();
	static int Frames = 0;

	double Now = glfwGetTime();
	Frames++;

	if (Now - LastTime >= 1.0)
	{
		const double FPS = Frames / (Now - LastTime);
		LastTime = Now;
		Frames = 0;

		glfwSetWindowTitle(MainWindowPtr, std::format("FPS: {}", static_cast<int>(std::floor(FPS))).c_str());
	}
}