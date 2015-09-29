#include "SpriteBatch.h"

#include <algorithm>

namespace GTEngine {

	SpriteBatch::SpriteBatch() : _vbo(0), _vao(0)
{
}


SpriteBatch::~SpriteBatch()
{
}


void SpriteBatch::init() {
	createVertexArray();

}

void SpriteBatch::begin(GlyphSortType sortType /* GlyphSortType::TEXTURE */) {
	_sortType = sortType;
	_renderBatches.clear();

	// Have to delete any glyphs that remain so we don't have memory leaks!
	for (int i = 0; i < _glyphs.size(); i++) {
		delete _glyphs[i];
	}

	_glyphs.clear();
}

void SpriteBatch::end() {
	sortGlyphs();
	createRenderBatches();
}

void SpriteBatch::draw(const glm::vec4& destRec, const glm::vec4& uvRect, GLuint texture, const Color& color, float depth) {
	Glyph* newGlyph = new Glyph;
	newGlyph->texture = texture;
	newGlyph->depth = depth;

	newGlyph->topLeft.color = color;
	newGlyph->topLeft.setPosition(destRec.x, destRec.y + destRec.w);
	newGlyph->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

	newGlyph->bottomLeft.color = color;
	newGlyph->bottomLeft.setPosition(destRec.x, destRec.y);
	newGlyph->bottomLeft.setUV(uvRect.x, uvRect.y);

	newGlyph->bottomRight.color = color;
	newGlyph->bottomRight.setPosition(destRec.x + destRec.z, destRec.y);
	newGlyph->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

	newGlyph->topRight.color = color;
	newGlyph->topRight.setPosition(destRec.x + destRec.z, destRec.y + destRec.w);
	newGlyph->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

	_glyphs.push_back(newGlyph);
}

void SpriteBatch::renderBatch() {

	// Bind our VAO. This sets up the opengl state we need, including the 
	// vertex attribute pointers and it binds the VBO
	glBindVertexArray(_vao);

	for (int i = 0; i < _renderBatches.size(); i++) {
		glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);

		glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
	}

	glBindVertexArray(0);
}

void SpriteBatch::createVertexArray() {

	// Generate the VAO and VBO if they are not generated arleady
	if (_vao == 0) {
		glGenVertexArrays(1, &_vao);
	}
	glBindVertexArray(_vao);

	if (_vbo == 0) {
		glGenBuffers(1, &_vbo);
	}
	// Everytime we rebind vertexAttribArray it is going to automaticly bind the buffer for us (we don't have to call bindBuffer anymore)
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	// Sending one vertex attribute array (we only have position);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// Tell the OpenGL where the vertex data is in the vertex buffer (point to the start of data if you want to draw everything from the buffer)
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));		// Normalize the RGB color 255-0 to 1-0

	// UV attribute pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	// Unbind all the buffer and vertexAtrib (everything above this line)
	glBindVertexArray(0);

}

void SpriteBatch::createRenderBatches() {
	// This will store all the vertices that we need to upload
	std::vector <Vertex> vertices;

	// Resize the buffer to the exact size we need so we can treat
	// it like an array
	vertices.resize(_glyphs.size() * 6);

	if (_glyphs.empty()) {
		return;
	}

	// Creates a new object with the given parameters
	_renderBatches.emplace_back(0, 6, _glyphs[0]->texture);

	// Current vertex
	int cv = 0, offset = 0;
	vertices[cv++] = _glyphs[0]->topLeft;
	vertices[cv++] = _glyphs[0]->bottomLeft;
	vertices[cv++] = _glyphs[0]->bottomRight;
	vertices[cv++] = _glyphs[0]->bottomRight;
	vertices[cv++] = _glyphs[0]->topRight;
	vertices[cv++] = _glyphs[0]->topLeft;
	offset += 6;


	// Current Glyph = cg
	for (int cg = 1; cg < _glyphs.size(); cg++) {

		// Check if this glyph can be part of the current batch
		if (_glyphs[cg]->texture != _glyphs[cg - 1]->texture) {
			// Creates a new object with the given parameters
			_renderBatches.emplace_back(offset, 6, _glyphs[cg]->texture);
		}
		else {
			// If its part of the current batch, just increase numVertices
			_renderBatches.back().numVertices += 6;
		}

		vertices[cv++] = _glyphs[cg]->topLeft;
		vertices[cv++] = _glyphs[cg]->bottomLeft;
		vertices[cv++] = _glyphs[cg]->bottomRight;
		vertices[cv++] = _glyphs[cg]->bottomRight;
		vertices[cv++] = _glyphs[cg]->topRight;
		vertices[cv++] = _glyphs[cg]->topLeft;

		offset += 6;
	}

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	// Orphan the buffer
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

	// Upload the data
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

	// Unbind the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void SpriteBatch::sortGlyphs() {
	// Elements with the same value keep their order => STABLE_SORT
	switch (_sortType) {
	case GlyphSortType::BACK_TO_FRONT:
		std::stable_sort(_glyphs.begin(), _glyphs.end(), 
			// Lambda expression begins
			[](Glyph* a, Glyph* b) {
			return (a->depth < b->depth);
		} /* end of lambda expression*/);
		break;
	case GlyphSortType::FRONT_TO_BACK:
		std::stable_sort(_glyphs.begin(), _glyphs.end(),
			[](Glyph* a, Glyph* b) {
			return (a->depth > b->depth);
		});
		break;
	case GlyphSortType::TEXTURE:
		std::stable_sort(_glyphs.begin(), _glyphs.end(),
			[](Glyph* a, Glyph* b) {
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
