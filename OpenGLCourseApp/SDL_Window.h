#pragma once

#include "SDL.h"
#include "GL/glew.h"

class SDL_Window
{
 public:
  SDL_Window();
  SDL_Window(GLint window_width, GLint window_height);
  inline GLint GetBufferWidth() { return buffer_width; }
  inline GLint GetBufferHeight() { return buffer_height; }
  inline bool GetIsRunning() { return is_running; }
  void ProcessInput();

	int Initialize();
  inline void SwapBuffer() { SDL_GL_SwapWindow(window); }

  ~SDL_Window();
 private:
  SDL_Window* window;
  SDL_GLContext gl_context;
  GLint width, height;
  GLint buffer_height, buffer_width; // viewport size in pixels
  bool is_running;

  void Destroy();
};

