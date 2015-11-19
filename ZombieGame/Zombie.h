#pragma once

#include <vector>
#include <glm/glm.hpp>

#include <GTEngine/SpriteBatch.h>


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

	// Getters
	glm::vec2 getPosition() const { return _position;  }

private:

	glm::vec2 _position;
};

