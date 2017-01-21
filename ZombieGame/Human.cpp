#include "Human.h"

#include <time.h>
#include <SDL/SDL.h>

#include <glm/gtx/string_cast.hpp>


Human::Human(int x, int y)
{
	m_position.x = x;
	m_position.y = y;
}


Human::~Human()
{
}

void Human::init(int x, int y) {
	m_position.x = x;
	m_position.y = y;
}

// Returns true if the human is dead
bool Human::update(World& world, std::vector<Bullet>& bullets) {

	// Check if the human should die
	for (int i = 0; i < bullets.size(); i++) {
		int bufferZone = 30;
		//std::cout << glm::to_string(bullets[i].getPosition()) << " == " << glm::to_string(_position) << std::endl;
		if ((bullets[i].getPosition().x < m_position.x + bufferZone) &&
			(bullets[i].getPosition().x > m_position.x - bufferZone) &&
			(bullets[i].getPosition().y < m_position.y + bufferZone) &&
			(bullets[i].getPosition().y > m_position.y - bufferZone)) {
			return true;
		}
	}

	// Initialise variables
	int bufferSpace{ 49 };
	int minBufferSpace{ 1 };
	int moveSpeed{ 1 };
	int chaos{ 2 };

	
	for (int i{ 0 }; i < chaos; ++i) {
		int direction = rand() % 4 + 0;
		// Move the humans
		if (direction == 0 && world.collision(m_position.x + bufferSpace, m_position.y)) {
			m_position.x += moveSpeed;
		}
		else if (direction == 1 && world.collision(m_position.x - minBufferSpace, m_position.y)) {
			m_position.x -= moveSpeed;
		}
		else if (direction == 2 && world.collision(m_position.x, m_position.y + bufferSpace)) {
			m_position.y += moveSpeed;
		}
		else if (direction == 3 && world.collision(m_position.x, m_position.y - minBufferSpace)) {
			m_position.y -= moveSpeed;
		}

	}

	return false;
}

void Human::draw(GTEngine::SpriteBatch& spriteBatch) {
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static GTEngine::GLTexture texture = GTEngine::ResourceManager::getTexture("Textures/circle.png");

	// Set the color to purple
	GTEngine::ColorRGBA8 colorPurple = GTEngine::ColorRGBA8(255, 5, 255, 255);

	glm::vec4 posAndSize = glm::vec4(m_position.x, m_position.y, 45, 45);

	// Draw the sprite
	spriteBatch.draw(posAndSize, uv, texture.id, colorPurple, 0.0f);
}