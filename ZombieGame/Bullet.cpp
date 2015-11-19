#include "Bullet.h"

#include <GTEngine/ResourceManager.h>

Bullet::Bullet(glm::vec2 post, glm::vec2 dir, float speed, int lifeTime)
{
	_lifeTime = lifeTime;
	_position = post;
	_direction = dir;
	_speed = speed;
}


Bullet::~Bullet()
{
}

void Bullet::draw(GTEngine::SpriteBatch& spriteBatch) {
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static GTEngine::GLTexture texture = GTEngine::ResourceManager::getTexture("Textures/bullet.png");

	GTEngine::ColorRGBA8 color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	glm::vec4 posAndSize = glm::vec4(_position.x, _position.y, 30, 30);

	// Draw the sprite
	spriteBatch.draw(posAndSize, uv, texture.id, color, 0.0f);
}

// Returns true if the bullet is dead
bool Bullet::update(World& world) {

	int bufferZone = 25;
	bool alive = world.collision(_position.x, _position.y);

	_position += _direction * _speed;
	_lifeTime--;
	if (_lifeTime == 0)
		return true;
	if (alive == 0)
		return true;
	return false;
}
