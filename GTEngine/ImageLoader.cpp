#include "ImageLoader.h"
#include "picoPNG.h"
#include "IOManager.h"
#include "ErrorHandler.h"

namespace GTEngine {

	GLTexture ImageLoader::loadPNG(std::string filePath) {
		GLTexture texture = {};		// Initialise every variable to 0

		unsigned long width, height;

		std::vector<unsigned char> out;
		std::vector<unsigned char> in;

		// Load the PNG into a buffer
		if (IOManager::readFileToBuffer(filePath, in) == false) {
			fatalError("Failed to load PNG file " + filePath + " to buffer.");
		}

		// Decode the PNG and check for errors
		int errorCode = decodePNG(out, width, height, &(in[0]), in.size());
		if (errorCode != 0) {
			fatalError("decodePNG failed with error: " + std::to_string(errorCode));
		}

		// Create the OpenGL texture
		glGenTextures(1, &(texture.id));
		glBindTexture(GL_TEXTURE_2D, texture.id);

		// Upload the image data to texture (transfers the buffer to VRAM on the graphics card)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		// Filter the texture with linear interpolation
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);

		// Unbind the texture
		glBindTexture(GL_TEXTURE_2D, 0);

		texture.width = width;
		texture.height = height;

		return texture;
	}

}