#include "Bullet.h"

#include <GTEngine/ResourceManager.h>

Bullet::Bullet(int lifeTime, float speed, glm::vec2 direction, glm::vec2 position) :
	m_lifeTime(lifeTime),
	m_speed(speed),
	m_direction(direction),
	m_position(position) {}

Bullet::~Bullet()
{
}

void Bullet::draw(GTEngine::SpriteBatch& spriteBatch) {
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static GTEngine::GLTexture texture = GTEngine::ResourceManager::getTexture("Textures/bullet.png");

	GTEngine::ColorRGBA8 color(255, 255, 255, 255);
	glm::vec4 posAndSize = glm::vec4(m_position.x, m_position.y, 30, 30);

	spriteBatch.draw(posAndSize, uv, texture.id, color, 0.0f);
}

// Returns true if the bullet is dead
bool Bullet::update(World& world) {

	int bufferZone = 25;
	bool alive = world.collision(m_position.x, m_position.y);

	m_position += m_direction * m_speed;
	m_lifeTime--;
	if (m_lifeTime == 0)
		return true;
	if (alive == 0)
		return true;
	return false;
}
