#pragma once

#include <functional>
#include <glm/glm.hpp>
#include "Vertex.h"
#include "SpriteBatch.h"
#include "GLTexture.h"

namespace GTEngine {

	class Partical2D
	{
	public: 
		glm::vec2 position{ glm::vec2(0.0f) };
		glm::vec2 velocity{ glm::vec2(0.0f) };
		ColorRGBA8 color;
		float life{ 0.0f };
		float width{ 0.0f };
	};

	inline void defaultParticalUpdate(Partical2D& partical, float deltaTime) {
		partical.position += partical.velocity * deltaTime;
	}

	class ParticalBatch2D
	{
	public:
		ParticalBatch2D();
		~ParticalBatch2D();

		void init(
			const int maxParticals,
			const float decayRate,
			GLTexture texture,
			std::function<void(Partical2D&, float)> updateFunc = defaultParticalUpdate
			);

		void update(float deltaTime);

		void draw(SpriteBatch*  spriteBatch);

		void addPartical(
			const glm::vec2& position,
			const glm::vec2& velocity, 
			const ColorRGBA8& color,
			float width
			);

	private:
		int findFreePartical();

		std::function<void(Partical2D&, float)> m_updateFunc;
		float m_decayRate{ 0.1f };
		Partical2D* m_particals = nullptr;
		int m_maxParticals{ 0 };
		int m_lastFreePartical{ 0 };
		GLTexture m_texture;
	};

}