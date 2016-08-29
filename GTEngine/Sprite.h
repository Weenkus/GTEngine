#pragma once

#include <GL/glew.h>
#include "GLTexture.h"

#include <string>

namespace GTEngine {

	class Sprite
	{
	public:
		Sprite();
		~Sprite();

		void init(float x, float y, float width, float height, std::string texturePath);
		void draw();

	private:
		int m_x;
		int m_y;
		int m_width;
		int m_height;
		GLuint m_vboID{ 0 };
		GLTexture m_texture;
	};

}
