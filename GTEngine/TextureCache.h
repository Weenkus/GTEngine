#pragma once

#include "GLTexture.h"

#include <map>
#include <string>

namespace GTEngine {

	class TextureCache
	{
	public:
		TextureCache();
		~TextureCache();

		GLTexture getTexture(std::string texturePath);

	private:
		std::map<std::string, GLTexture> _textureMap;
	};

}

