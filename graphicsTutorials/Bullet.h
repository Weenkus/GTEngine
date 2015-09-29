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
	int _lifeTime;
	float _speed;
	glm::vec2 _direction;
	glm::vec2 _position;
};
