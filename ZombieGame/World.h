#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include <GTEngine/SpriteBatch.h>

#include <glm/glm.hpp>

const int MAP_HEIGHT = 32;
const int MAP_WIDTH = 112;

enum direction {LEFT, RIGHT, UP, DOWN};

class World
{
public:
	World();
	~World();

	void init();
	void draw(GTEngine::SpriteBatch& spriteBatch);
	void printWorldMap();

	bool collision(int x, int y);



	// getters
	glm::vec2 getPlayerStartPos() const { return _playerStart; }
	int getHumanNumber() const { return _numHumans; }
	int getZombieNumber() const { return _numZombies; }

	// setters
	void setHumanNumber(int humanNumber) { _numHumans = humanNumber; }
	void setZombieNumber(int zombieNumber) { _numHumans = zombieNumber; }
	

	char world[MAP_HEIGHT][MAP_WIDTH];

private:

	// Memeber function
	void read();
	void parse();

	// Fields
	std::vector<std::string> _textParse;

	int _numHumans;
	int _numZombies;

	glm::vec2 _playerStart;
};

