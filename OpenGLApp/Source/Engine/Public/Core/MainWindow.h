#pragma once

#include <GL/glew.h>

#include <GLFW/glfw3.h>

class MainWindow
{
public:
	MainWindow(GLint Width, GLint Height);
	~MainWindow();

	int Initialize();

	GLfloat GetBufferWidth() const;
	GLfloat GetBufferHeight() const ;

	void SwapBuffers();
	bool GetShouldClose();

	bool* GetKeys();

	GLdouble GetOffsetX();
	GLdouble GetOffsetY();

	void ShowFPS() const;

private:
	GLFWwindow* MainWindowPtr;

	GLint Width, Height;
	GLint BufferWidth, BufferHeight;

	// Mouse input handling
	bool IsMouseMovedFirst;

	GLdouble LastX;
	GLdouble LastY;
	GLdouble OffsetX;
	GLdouble OffsetY;

	static void HandleMouse(GLFWwindow* Window, GLdouble XPos, GLdouble YPos);

	// Keyboard input handling
	bool Keys[1024];
	static void HandleKeys(GLFWwindow* Window, int Key, int ScanCode, int Action, int Mode);
};
