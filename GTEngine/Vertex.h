#pragma once

#include <GL/glew.h>

namespace GTEngine {

	struct Position {
		float x;
		float y;
	};

	struct Color {
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	};

	struct UV {
		float u;
		float v;
	};

	class Vertex {

		// We want our vertex memory to be dividable by 4
	public:
		Position position;
		Color color;

		// UV texture coordinates
		UV uv;

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a);

		void setPosition(float x, float y);

		void setUV(float u, float v);
	};

}