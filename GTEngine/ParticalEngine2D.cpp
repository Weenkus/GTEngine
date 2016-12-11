#include "ParticalEngine2D.h"

#include "ParticalBatch2D.h"
#include "SpriteBatch.h"

namespace GTEngine {

	ParticalEngine2D::ParticalEngine2D() {
	} 


	ParticalEngine2D::~ParticalEngine2D() {
		for (auto& batch : m_batches) {
			delete batch;
		}
	}
	

	void ParticalEngine2D::addParticalBatch(ParticalBatch2D* particalBatch) {
		m_batches.push_back(particalBatch);
	}

	void ParticalEngine2D::update(float deltaTime) {
		for (auto& batch : m_batches) {
			batch->update(deltaTime);
		}
	}

	void ParticalEngine2D::draw(SpriteBatch* spriteBatch) {
		for (auto& batch : m_batches) {
			spriteBatch->begin();
			batch->draw(spriteBatch);
			spriteBatch->end();
			spriteBatch->renderBatch();
		}
		
	}
}
