#include "SpriteBatch.h"

#include <algorithm>

namespace GTEngine {

	SpriteBatch::SpriteBatch() : m_vbo(0), m_vao(0)
{
}


SpriteBatch::~SpriteBatch()
{
}


void SpriteBatch::init() {
	createVertexArray();
}

void SpriteBatch::begin(GlyphSortType sortType /* GlyphSortType::TEXTURE */) {
	m_sortType = sortType;
	m_renderBatches.clear();
	m_glyphs.clear();
}

void SpriteBatch::end() {
	m_glyphPointers.resize(m_glyphs.size());

	for (int i{ 0 }; i < m_glyphs.size(); ++i)
		m_glyphPointers[i] = &m_glyphs[i];

	sortGlyphs();
	createRenderBatches();
}

void SpriteBatch::draw(const glm::vec4& destRec, const glm::vec4& uvRect, GLuint texture, const ColorRGBA8& color, float depth) {
	m_glyphs.emplace_back(destRec, uvRect, texture, color, depth);
}

void SpriteBatch::renderBatch() {

	// Bind our VAO. This sets up the opengl state we need, including the 
	// vertex attribute pointers and it binds the VBO
	glBindVertexArray(m_vao);
	{
		for (int i = 0; i < m_renderBatches.size(); i++) {
			glBindTexture(GL_TEXTURE_2D, m_renderBatches[i].texture);
			glDrawArrays(GL_TRIANGLES, m_renderBatches[i].offset, m_renderBatches[i].numVertices);
		}
	}
	glBindVertexArray(0);
}

void SpriteBatch::createVertexArray() {

	// Generate the VAO and VBO if they are not generated arleady
	if (m_vao == 0) {
		glGenVertexArrays(1, &m_vao);
	}

	glBindVertexArray(m_vao);
	{
		if (m_vbo == 0) {
			glGenBuffers(1, &m_vbo);
		}
		// Everytime we rebind vertexAttribArray it is going to automaticly bind the buffer for us (we don't have to call bindBuffer anymore)
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		// Sending one vertex attribute array (we only have position);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		// Tell the OpenGL where the vertex data is in the vertex buffer (point to the start of data if you want to draw everything from the buffer)
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		// Normalize the RGB color 255-0 to 1-0		
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));																					// UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	}
	// Unbind all the buffer and vertexAtrib (everything above this line)
	glBindVertexArray(0);

}

void SpriteBatch::createRenderBatches() {
	// This will store all the vertices that we need to upload
	std::vector <Vertex> vertices;

	// Resize the buffer to the exact size we need so we can treat
	// it like an array
	vertices.resize(m_glyphs.size() * 6);

	if (m_glyphPointers.empty()) {
		return;
	}

	// Creates a new object with the given parameters
	m_renderBatches.emplace_back(0, 6, m_glyphPointers[0]->texture);

	// Current vertex
	int currentVertex{ 0 }, offset{ 0 };
	vertices[currentVertex++] = m_glyphPointers[0]->topLeft;
	vertices[currentVertex++] = m_glyphPointers[0]->bottomLeft;
	vertices[currentVertex++] = m_glyphPointers[0]->bottomRight;
	vertices[currentVertex++] = m_glyphPointers[0]->bottomRight;
	vertices[currentVertex++] = m_glyphPointers[0]->topRight;
	vertices[currentVertex++] = m_glyphPointers[0]->topLeft;
	offset += 6;


	// Current Glyph = cg
	for (int currentGlyph{ 1 }; currentGlyph < m_glyphPointers.size(); currentGlyph++) {
		// Check if this glyph can be part of the current batch
		if (m_glyphPointers[currentGlyph]->texture != m_glyphPointers[currentGlyph - 1]->texture) {
			// Creates a new object with the given parameters
			m_renderBatches.emplace_back(offset, 6, m_glyphPointers[currentGlyph]->texture);
		}
		else {
			// If its part of the current batch, just increase numVertices
			m_renderBatches.back().numVertices += 6;
		}

		vertices[currentVertex++] = m_glyphPointers[currentGlyph]->topLeft;
		vertices[currentVertex++] = m_glyphPointers[currentGlyph]->bottomLeft;
		vertices[currentVertex++] = m_glyphPointers[currentGlyph]->bottomRight;
		vertices[currentVertex++] = m_glyphPointers[currentGlyph]->bottomRight;
		vertices[currentVertex++] = m_glyphPointers[currentGlyph]->topRight;
		vertices[currentVertex++] = m_glyphPointers[currentGlyph]->topLeft;
		offset += 6;
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	{
		// Orphan the buffer
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

		// Upload the data
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void SpriteBatch::sortGlyphs() {
	// Elements with the same value keep their order => STABLE_SORT
	switch (m_sortType) {
	case GlyphSortType::BACK_TO_FRONT:
		std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), [](Glyph* a, Glyph* b) {
			return (a->depth < b->depth);
		});
		break;
	case GlyphSortType::FRONT_TO_BACK:
		std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), [](Glyph* a, Glyph* b) 
		{
			return (a->depth > b->depth);
		});
		break;
	case GlyphSortType::TEXTURE:
		std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), [](Glyph* a, Glyph* b) {
			return (a->texture < b->texture);
		});
		break;
	}
	
}

bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b) {
	return (a->depth < b->depth);
}
bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b){
	return (a->depth > b->depth);
}
bool SpriteBatch::compareTexture(Glyph* a, Glyph* b){
	return (a->texture < b->texture);
}

}
