#include "ParticalBatch2D.h"

namespace GTEngine {

	ParticalBatch2D::ParticalBatch2D() {

	}


	ParticalBatch2D::~ParticalBatch2D() {
		delete[] m_particals;
	}

	void ParticalBatch2D::init(
		const int maxParticals,
		const float decayRate,
		GLTexture texture,
		std::function<void(Partical2D&, float)> updateFunc /* = defaultParticalUpdate */
		) {
		m_decayRate = decayRate;
		m_maxParticals = maxParticals;
		m_particals = new Partical2D[maxParticals];
		m_texture = texture;
		m_updateFunc = updateFunc;
	}


	void ParticalBatch2D::update(float deltaTime) {
		for (int i{ 0 }; i < m_maxParticals; ++i) {
			auto& partical = m_particals[i];
			if (partical.life > 0.0f) {
				m_updateFunc(partical, deltaTime);
				partical.life -= m_decayRate * deltaTime;
			}
		}
	}

	void ParticalBatch2D::draw(SpriteBatch* spriteBatch) {
		glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		for (int i{ 0 }; i < m_maxParticals; ++i) {
			auto& partical = m_particals[i];
			if (partical.life > 0.0f) {
				glm::vec4 destRect(partical.position.x, partical.position.y, partical.width, partical.width);
				spriteBatch->draw(destRect, uvRect, m_texture.id, partical.color, 0.0f);
			}
		}
	}

	void ParticalBatch2D::addPartical(
		const glm::vec2& position,
		const glm::vec2& velocity,
		const ColorRGBA8& color,
		float width) {

		int particalIndex = findFreePartical();
		auto& partical = m_particals[particalIndex];

		partical.life = 1.0f;
		partical.color = color;
		partical.position = position;
		partical.velocity = velocity;
		partical.width = width;
	}

	int ParticalBatch2D::findFreePartical() {
		for (int i{ m_lastFreePartical }; i < m_maxParticals; ++i) {
			if (m_particals[i].life <= 0.0f) {
				m_lastFreePartical = i;
				return i;
			}
		}

		for (int i{ 0 }; i < m_lastFreePartical; ++i) {
			if (m_particals[i].life <= 0.0f) {
				m_lastFreePartical = i;
				return i;
			}
		}

		// No free particals, overwrite the first one
		return 0;
	}

}
