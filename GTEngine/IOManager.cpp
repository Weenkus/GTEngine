#include "IOManager.h"

#include <fstream>

namespace GTEngine {

	bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer) {

		// Read the file in binary
		std::ifstream file(filePath, std::ios::binary);
		if (file.fail()) {
			perror(filePath.c_str());
			return false;
		}

		// Seek to the end of a file
		file.seekg(0, std::ios::end);

		// Get the file size
		int fileSize = file.tellg();
		file.seekg(0, std::ios::beg); 	// Restor the position in the file

		fileSize -= file.tellg();	// Disregard the header bytes of the file

		buffer.resize(fileSize);
		file.read((char *)&(buffer[0]), fileSize);
		file.close();

		return true;
	}

}
