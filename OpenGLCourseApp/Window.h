#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	int initialize();

	inline GLfloat getBufferWidth() { return bufferWidth; }
	inline GLfloat getBufferHeight() { return bufferHeight; }

	inline bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	void swapBuffer() { glfwSwapBuffers(mainWindow); }

	~Window();

private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;
};

