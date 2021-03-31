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

	bool* getKeys() { return keys; }
	GLfloat getXChange(); // these are simplifications. see vid 3.19 for explanation of better method
	GLfloat getYChange();

	inline bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	void swapBuffer() { glfwSwapBuffers(mainWindow); }

	~Window();

private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024]; // covers range of asci characters

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved; // prevent a glitch when program starts (don't react to initial mouse position)

	void createCallbacks();
	// This is static because GLFW callback does not work with regular member functions of the same class
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};

