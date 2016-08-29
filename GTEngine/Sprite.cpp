#include "Sprite.h"
#include "Vertex.h"
#include "ResourceManager.h"

#include <cstddef>

namespace GTEngine {

	Sprite::Sprite()
	{
		// Initialise the vertex buffer
		m_vboID = 0;
	}


	Sprite::~Sprite()
	{
		// Free the buffer
		if (m_vboID != 0) {
			glDeleteBuffers(1, &m_vboID);
		}
	}

	void Sprite::init(float x, float y, float width, float height, std::string texturePath){
		m_x = x;
		m_y = y;
		m_width = width;
		m_height = height;

		m_texture = ResourceManager::getTexture(texturePath);

		// Create the vertex object if it does not exist
		if (m_vboID == 0) {
			glGenBuffers(1, &m_vboID);
		}

		Vertex vertexData[6];	// 6 vertices in 2D space

		// First triangle
		vertexData[0].setPosition(x + width, y + height);		// top right vertex	
		vertexData[0].setUV(1.0f, 1.0f);

		vertexData[1].setPosition(x, y + height);				// top left vertex
		vertexData[1].setUV(0.0f, 1.0f);

		vertexData[2].setPosition(x, y);						// bottom left vertex
		vertexData[2].setUV(0.0f, 0.0f);

		// Second triangle
		vertexData[3].setPosition(x, y);						// bottom left vertex		
		vertexData[3].setUV(0.0f, 0.0f);

		vertexData[4].setPosition(x + width, y);				// bottom right vertex
		vertexData[4].setUV(1.0f, 0.0f);

		vertexData[5].setPosition(x + width, y + height);		// top right vertex
		vertexData[5].setUV(1.0f, 1.0f);

		for (int i = 0; i < 6; i++) {
			vertexData[i].setColor(255, 0, 255, 255);
		}

		vertexData[1].setColor(0, 0, 255, 255);
		vertexData[4].setColor(0, 255, 0, 255);

		// Bind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

		// Upload the data to the GPU (from the CPU)
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

		// Unbind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Sprite::draw() {

		glBindTexture(GL_TEXTURE_2D, m_texture.id);	// Don't unbind the texture, not sure if other spirtes might use it

		// Bind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

		// Sending one vertex attribute array (we only have position);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		// Tell the OpenGL where the vertex data is in the vertex buffer (point to the start of data if you want to draw everything from the buffer)
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));		// Normalize the RGB color 255-0 to 1-0

		// UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		// Draw the data
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// Unbind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}
