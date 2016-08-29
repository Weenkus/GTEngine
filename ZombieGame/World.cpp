#include "World.h"

#include <GTEngine/ResourceManager.h>



World::World()
{
}


World::~World()
{
}

void World::init() {
	read();
	parse();
}

// Parse the file and create the world
void World::read() {
	std::string world;
	std::string line;
	std::ifstream myfile("Levels/level1.txt");
	if (myfile.is_open())
	{

		while (getline(myfile, line))
		{
			m_textParse.push_back(line);
		}
		myfile.close();
	}
}

void  World::parse() {
	int k = 0;
	for (std::vector<std::string>::const_iterator i = m_textParse.begin(); i != m_textParse.end(); ++i) {
		// Get the number of humans
		if (i == m_textParse.begin()) {
			std::string number = (*i).substr(8, (*i).size());
			std::cout << "Humans: " + number + "\n";
			std::string::size_type sz;   // alias of size_t
			m_numHumans = std::stoi(number, &sz);
		}
		// Create the world
		else {
			for (int j = 0; j < (*i).length(); j++) {
				world[k][j] = (*i).at(j);
				// Remember the play starting position
				if (world[k][j] == '@') {
					m_playerStart.x = j;
					m_playerStart.y = k;
				}
			}
			++k;
		}	
	}
	//printWorldMap();
}

void World::draw(GTEngine::SpriteBatch& spriteBatch) {
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static GTEngine::GLTexture textureWallRed = GTEngine::ResourceManager::getTexture("Textures/red_bricks.png");
	static GTEngine::GLTexture textureWallLight = GTEngine::ResourceManager::getTexture("Textures/light_bricks.png");
	static GTEngine::GLTexture textureWallGLass = GTEngine::ResourceManager::getTexture("Textures/glass.png");

	GTEngine::ColorRGBA8 color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	//Draw the map
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			glm::vec4 posAndSize = glm::vec4(j*50, i*50, 50, 50);
			if (world[i][j] == 'R' || world[i][j] == 'B') {
				spriteBatch.draw(posAndSize, uv, textureWallRed.id, color, 0.0f);
			}
			if (world[i][j] == 'B' || world[i][j] == 'L') {
				spriteBatch.draw(posAndSize, uv, textureWallLight.id, color, 0.0f);
			}
			if (world[i][j] == 'G') {
				spriteBatch.draw(posAndSize, uv, textureWallGLass.id, color, 0.0f);
			}
		}
	}
}

bool World::collision(int x, int y) {
	x /= 50;
	y /= 50;

	if (world[y][x] == 'R' || world[y][x] == 'L' || world[y][x] == 'B' || world[y][x] == 'G') {
		// Check if a zombie or humans is there
		return false;
	}
	return true;
}

void World::printWorldMap() {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			std::cout << world[i][j];
		}
	}
	std::cout << "\n";
}

