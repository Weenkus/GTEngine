#include "ResourceManager.h"

namespace GTEngine {

	TextureCache ResourceManager::_textureCache;		// Because the variable is static it needed redeclaring in the .cpp


	GLTexture ResourceManager::getTexture(std::string texturePath) {
		return _textureCache.getTexture(texturePath);
	}

}