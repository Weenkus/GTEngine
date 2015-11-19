#include <SDL/SDL.h>
#include <GL/glew.h>

namespace GTEngine {

	int init() {
		// Initialise SDL
		SDL_Init(SDL_INIT_EVERYTHING);

		// Tell OpenGL that we want to use the double buffer
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return 0;
	}

}