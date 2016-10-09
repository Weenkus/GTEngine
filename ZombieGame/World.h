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

	glm::vec2 getPlayerStartPos() const { return m_playerStart; }
	int getHumanNumber() const { return m_numHumans; }
	int getZombieNumber() const { return m_numZombies; }

	void setHumanNumber(int humanNumber) { m_numHumans = humanNumber; }
	void setZombieNumber(int zombieNumber) { m_numHumans = zombieNumber; }
	

	char world[MAP_HEIGHT][MAP_WIDTH];

private:

	void read();
	void parse();

	std::vector<std::string> m_textParse;

	int m_numHumans;
	int m_numZombies;

	glm::vec2 m_playerStart;
};

