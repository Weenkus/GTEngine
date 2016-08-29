#pragma once

#include <GTEngine/SpriteBatch.h>

#include <glm/glm.hpp>
#include <vector>

#include "World.h"
#include "Bullet.h"
#include "Human.h"


class Zombie
{
public:
	Zombie(int x, int y);
	~Zombie();

	bool update(World& world, std::vector<Bullet>& bullets, std::vector<Human>& humans);
	void draw(GTEngine::SpriteBatch& spriteBatch);

	glm::vec2 getPosition() const { return m_position;  }

private:
	glm::vec2 m_position;
};

