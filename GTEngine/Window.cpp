#include "Window.h"
#include "ErrorHandler.h"

namespace GTEngine {

	Window::Window()
	{
	}


	Window::~Window()
	{
	}

	int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags) {

		// Check user flags and set OpenGL flags accordingly
		Uint32 flags{ SDL_WINDOW_OPENGL };

		if (currentFlags & INVISIBLE) {
			flags |= SDL_WINDOW_HIDDEN;
		}
		if (currentFlags & FULLSCREEN) {
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (currentFlags & BORDERLESS) {
			flags |= SDL_WINDOW_BORDERLESS;
		}

		// Create the window for our game engine
		m_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);
		if (m_sdlWindow == nullptr) {
			fatalError("SDL Window could not be created!");
		}

		// Save the OpenGL context to the game engine window
		SDL_GLContext glContex = SDL_GL_CreateContext(m_sdlWindow);
		if (glContex == nullptr) {
			fatalError("SDL_GL contect could not be created!");
		}

		// Initialise the GLEW library
		GLenum error = glewInit();
		if (error != GLEW_OK) {
			fatalError("GLEW could not be initialise!");
		}

		// Check the OpenGL version
		std::printf("*** OpenGL Version: %s   ***\n", glGetString(GL_VERSION));

		// Set the background to blue
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

		// Turn on VSync (1) or turn off (0)
		SDL_GL_SetSwapInterval(0);

		// Enable alfa blend
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		return 0;
	}

	void Window::swapBuffer() {
		SDL_GL_SwapWindow(m_sdlWindow);
	}

}