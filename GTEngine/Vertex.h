#pragma once

#include <GL/glew.h>

namespace GTEngine {

	struct Position {
		float x;
		float y;
	};

	struct ColorRGBA8 {

		// Set the default color to black
		ColorRGBA8() : r(0), g(0), b(0), a(0) {}

		// Color constructor
		ColorRGBA8(GLubyte R, GLubyte G, GLubyte B, GLubyte A) : r(R), g(G), b(B), a(A) {}

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
		ColorRGBA8 color;
		UV uv;

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a);
		void setPosition(float x, float y);
		void setUV(float u, float v);
	};

}