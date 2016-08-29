#include "TextureCache.h"
#include "ImageLoader.h"

#include <iostream>

namespace GTEngine {

	TextureCache::TextureCache()
	{
	}


	TextureCache::~TextureCache()
	{
	}


	GLTexture TextureCache::getTexture(std::string texturePath){

		// Lookup the texture and see if its in the map
		auto mit = m_textureMap.find(texturePath);		// auto == std::map<std::string, GLTexture>::iterator

		// Check if its not in the map
		if (mit == m_textureMap.end()) {
			GLTexture newTexture = ImageLoader::loadPNG(texturePath);

			// Insert the new texture inside the map
			m_textureMap.insert(make_pair(texturePath, newTexture));

			std::cout << "Loaded Texture.\n";
			return newTexture;
		}

		std::cout << "Used Cached Texture.\n";
		return mit->second;

	}

}
