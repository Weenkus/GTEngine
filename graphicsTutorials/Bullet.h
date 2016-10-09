#pragma once

#include <glm/glm.hpp>

#include <GTEngine/SpriteBatch.h>


class Bullet
{
public:
	Bullet(glm::vec2 post, glm::vec2 dir, float speed, int _lifeTime);
	~Bullet();

	void draw(GTEngine::SpriteBatch& spriteBatch);

	// Returns true when we are out of life
	bool update();


private:
	int m_lifeTime;
	float m_speed;
	glm::vec2 m_direction;
	glm::vec2 m_position;
};

