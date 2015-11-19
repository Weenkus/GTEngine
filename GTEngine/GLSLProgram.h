#pragma once

#include <string>
#include <GL/glew.h>

namespace GTEngine {

	class GLSLProgram
	{
	public:
		GLSLProgram();
		~GLSLProgram();

		void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentSaderFilePath);

		void linkShaders();

		void compileShader(const std::string &filePath, GLuint id);

		void addAttribute(const std::string& attributeName);

		GLint getUniformLocation(const std::string& uniformName);

		void use();
		void unuse();

	private:

		int _numAttributes;

		GLuint _programID;		// Handel that refers to the entire program

		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;

	};

}

