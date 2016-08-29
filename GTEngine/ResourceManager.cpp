#include "ResourceManager.h"

namespace GTEngine {

	TextureCache ResourceManager::m_textureCache;		// Because the variable is static it needed redeclaring in the .cpp


	GLTexture ResourceManager::getTexture(std::string texturePath) {
		return m_textureCache.getTexture(texturePath);
	}

}