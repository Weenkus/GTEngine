#pragma once


#include <glm/glm.hpp>
#include <GTEngine/ResourceManager.h>

#include "World.h"
#include "Bullet.h"



class Human
{
public:
	Human(int x, int y);
	~Human();

	void init(int x, int y);
	bool update(World& world, std::vector<Bullet>& bullets);
	void draw(GTEngine::SpriteBatch& spriteBatch);

	// Getters
	glm::vec2 getPosition() const { return _position;  }


private:
	glm::vec2 _position;
};

