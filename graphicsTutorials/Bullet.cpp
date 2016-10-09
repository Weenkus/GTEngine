#include "Bullet.h"

#include <GTEngine/ResourceManager.h>

Bullet::Bullet(glm::vec2 post, glm::vec2 dir, float speed, int lifeTime)
{
	m_lifeTime = lifeTime;
	m_position = post;
	m_direction = dir;
	m_speed = speed;
}


Bullet::~Bullet()
{
}

void Bullet::draw(GTEngine::SpriteBatch& spriteBatch) {
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static GTEngine::GLTexture texture = GTEngine::ResourceManager::getTexture("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

	GTEngine::ColorRGBA8 color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	glm::vec4 posAndSize = glm::vec4(m_position.x, m_position.y, 30, 30);

	// Draw the sprite
	spriteBatch.draw(posAndSize, uv, texture.id, color, 0.0f);
}

bool Bullet::update() {
	m_position += m_direction * m_speed;
	m_lifeTime--;
	if (m_lifeTime == 0)
		return true;
	return false;
}
