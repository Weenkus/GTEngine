#pragma once

#include <glm/glm.hpp>

#include <GTEngine/SpriteBatch.h>

#include "World.h"


class Bullet
{
public:
	Bullet(int lifeTime, float speed, glm::vec2 direction, glm::vec2 position);
	~Bullet();

	void draw(GTEngine::SpriteBatch& spriteBatch);

	// Returns true when we are out of life
	bool update(World& world);

	glm::vec2 getPosition() const { return m_position; }


private:
	int m_lifeTime;
	float m_speed;
	glm::vec2 m_direction;
	glm::vec2 m_position;
};

