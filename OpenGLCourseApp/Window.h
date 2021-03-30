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

	bool keys[1024]; // covers range of asci characters

	void createCallbacks();
	// This is static because GLFW callback does not work with regular member functions of the same class
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
};

