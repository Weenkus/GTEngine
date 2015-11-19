#include "Vertex.h"

namespace GTEngine {

	void Vertex::setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}

	void Vertex::setPosition(float x, float y) {
		position.x = x;
		position.y = y;
	}

	void Vertex::setUV(float u, float v) {
		uv.u = u;
		uv.v = v;
	}

}