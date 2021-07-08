#include <iostream>

#include "SDL_Window.h"
#include "SDL_ttf.h"

SDL_Window::SDL_Window() : height{ 600 }, width{ 800 }, window{ NULL }, buffer_height{ 600 }, buffer_width{ 600 }, gl_context{ NULL },
                          is_running{ false } {}

SDL_Window::SDL_Window(GLint window_width, GLint window_height)
    : height{window_height}, width{window_width}, window{NULL}, buffer_height{ 600 }, buffer_width{ 600 }, gl_context{ NULL },
													is_running{ false } {}

int SDL_Window::Initialize() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {             // Initialize all SDL subsystems & check for success
    fprintf(stderr, "ERROR: SDL failed to initialize: %s\n", SDL_GetError());
    return 1;
  }

  // initialize SDL for OpenGL 3.3
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);     // disable funtions depricated in specified version
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // in windows, disable future deprecated functionality

  window = SDL_CreateWindow(    // create window via SDL API
        NULL,                   // window title
        SDL_WINDOWPOS_CENTERED, // xpos of window
        SDL_WINDOWPOS_CENTERED, // ypos of window
        width,                  // width of window
        height,                 // height of window
        SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS); // flags (opengl usage, no border)

  if (!window) {
    fprintf(stderr, "ERROR: SDL window failed to initialize: %s\n", SDL_GetError());
    return 1;
  }

  SDL_GL_GetDrawableSize(window, &buffer_width, &buffer_height);

  gl_context = SDL_GL_CreateContext(window); // create OpenGL context associated with window
  if (!gl_context)
  {
    fprintf(stderr, "ERROR: OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
    return 1;
  }

  glewExperimental = GL_TRUE;
  GLenum glewError = glewInit();
  if (glewError != GLEW_OK)
  {
    fprintf(stderr, "ERROR: GLEW failed to initialize: %s\n", glewGetErrorString(glewError));
  }

  glEnable(GL_DEPTH_TEST); // test depth of each pixel for drawing

  glViewport(0, 0, buffer_width, buffer_height);

  is_running = true;

  return 0;
}

void SDL_Window::ProcessInput() {
  SDL_Event event;

  SDL_PollEvent(&event);

  switch (event.type) {
		case SDL_QUIT: {
			is_running = false;
			break;
		}
		case SDL_KEYDOWN: {
			if (event.key.keysym.sym == SDLK_ESCAPE ||
        event.key.keysym.sym == SDLK_q)
			{
				printf("keypress");
				is_running = false;
			}
      break;
		}
		default: {
			break;
		}
  }
}

void SDL_Window::Destroy() { 
  SDL_DestroyWindow(window); // free window memory
  SDL_Quit();                // shut down all SDL subsystems
}

SDL_Window::~SDL_Window() { 
  Destroy();
}
