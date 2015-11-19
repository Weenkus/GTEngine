#pragma once

#include "GLTexture.h"

#include <string>

namespace GTEngine {

	static class ImageLoader
	{
	public:
		static GLTexture loadPNG(std::string filePath);
	};

}

