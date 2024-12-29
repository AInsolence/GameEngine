#pragma once

#include <memory>
#include <stdio.h>
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

private:
	GLFWwindow* MainWindowPtr;

	GLint Width, Height;
	GLint BufferWidth, BufferHeight;
};

