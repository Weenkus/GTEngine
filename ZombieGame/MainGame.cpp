#include "MainGame.h"

#include <GTEngine/GTEngine.h>

#include <SDL/SDL.h>

#include <iostream>
#include <time.h>

// For sound
#include <Windows.h>
#include <mmsystem.h>

const float CAMERA_SPEED = 3.0f;
const float SCALE_SPEED = 0.1f;

MainGame::MainGame() : _screenWidth(1024), _screenHeight(768), _maxFPS(60), _gameState(GameState::PLAY)
{
	_camera.init(_screenWidth, _screenHeight);
}

MainGame::~MainGame() {
    // IMPLEMENT THIS!
}

void MainGame::run() {
	initSystems();
	gameLoop();
}

void MainGame::initSystems() {
	GTEngine::init();
	_window.create("Zombie Game0", _screenWidth, _screenHeight, 0);
	initShaders();
	_spriteBatch.init();
	_fpsLimiter.init(_maxFPS);
	_world.init();

	// Init agents
	initCameraAndPlayer();
	initHumans();
	initZombies();

	// Play Music
	PlaySound(TEXT("Sound/hoc.wav"), NULL, SND_ASYNC | SND_NOSTOP);
}

void MainGame::initShaders() {
    // Compile our color shader
    _textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
    _textureProgram.addAttribute("vertexPosition");
    _textureProgram.addAttribute("vertexColor");
    _textureProgram.addAttribute("vertexUV");
    _textureProgram.linkShaders();
}

void MainGame::gameLoop() {
	while (_gameState != GameState::EXIT) {
		_fpsLimiter.beginFrame();

		// Process user input
		processInput();

		// Update the game state
		_camera.update();
		for (int i = 0; i < _bullets.size();) {
			if (_bullets[i].update(_world) == true) {	// Erase the bullet 
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
			}
			else {
				i++;
			}
		}
		srand(time(NULL));
		for (int i = 0; i < _humans.size();) {
			if (_humans[i].update(_world, _bullets) == true) { // Kill the human
				_humans[i] = _humans.back();
				_humans.pop_back();
			}
			else {
				i++;
			}
		}
		for (int i = 0; i < _zombies.size();) {
			if (_zombies[i].update(_world, _bullets, _humans) == true) { // Kill the human
				_zombies[i] = _zombies.back();
				_zombies.pop_back();
			}
			else {
				i++;
			}
		}

		_inputManager.update();

		transformHumansToZombies(_humans, _zombies);

		// Render the game
		drawGame();

		// Limit and print the FPS
		float fps = _fpsLimiter.endFrame();
		int frameNumber = 200;
		printFPS(frameNumber, fps);
	}
}

void MainGame::processInput() {
    SDL_Event evnt;
    //Will keep looping until there are no more events to process
    while (SDL_PollEvent(&evnt)) {
        switch (evnt.type) {
            case SDL_QUIT:
				_gameState = GameState::EXIT;
                break;
            case SDL_MOUSEMOTION:
                _inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
                break;
            case SDL_KEYDOWN:
                _inputManager.pressKey(evnt.key.keysym.sym);
                break;
            case SDL_KEYUP:
                _inputManager.releaseKey(evnt.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                _inputManager.pressKey(evnt.button.button);
				// Play gunfire
				//PlaySound(TEXT("Sound/gun.mp3"), NULL, SND_ASYNC);
                break;
            case SDL_MOUSEBUTTONUP:
                _inputManager.releaseKey(evnt.button.button);
                break;
        }
    }

	// Allow user movement
	int bufferSpace = 49;
	int minBufferSpace = 1;
	if (_inputManager.isKeyDown(SDLK_w) && _world.collision(_playerPosition.x, _playerPosition.y + bufferSpace)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
		_playerPosition.y += CAMERA_SPEED;
	}
	if (_inputManager.isKeyDown(SDLK_s) && _world.collision(_playerPosition.x, _playerPosition.y - minBufferSpace)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
		_playerPosition.y -= CAMERA_SPEED;
	}
	if (_inputManager.isKeyDown(SDLK_a) && _world.collision(_playerPosition.x - minBufferSpace, _playerPosition.y)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
		_playerPosition.x -= CAMERA_SPEED;
	}
	if (_inputManager.isKeyDown(SDLK_d) && _world.collision(_playerPosition.x + bufferSpace, _playerPosition.y)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
		_playerPosition.x += CAMERA_SPEED;
	}
	if (_inputManager.isKeyDown(SDLK_q)) {
		_camera.setScale(_camera.getScale() + SCALE_SPEED);
	}
	if (_inputManager.isKeyDown(SDLK_e)) {
		_camera.setScale(_camera.getScale() - SCALE_SPEED);
	}

	// Shooting bullets
	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
		// Transfer window coordinates to world coordinates
		glm::vec2 mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = _camera.convertScreenToWorld(mouseCoords);
		//std::cout << mouseCoords.x << " " << mouseCoords.y << std::endl;

		// Shoot bullets to mouse pointer
		glm::vec2 playerPosition(0.0f);
		playerPosition.x = _playerPosition.x;
		playerPosition.y = _playerPosition.y;

		glm::vec2 direction = mouseCoords - playerPosition;
		direction = glm::normalize(direction);

		_bullets.emplace_back(playerPosition, direction, 5.01f, 1000);
	}
}

void MainGame::drawGame() {
    // Set the base depth to 1.0
    glClearDepth(1.0);
    // Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // IMPLEMENT THIS!
	_textureProgram.use();

	glActiveTexture(GL_TEXTURE0);		// Use only one texture

	// Set uniforms
	GLint textureLocation = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);		// Use the above activated texture0

	GLint pLocation = _textureProgram.getUniformLocation("P");		// Set the ortho matrix
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));		// Upload the matrix to the GPU

	_spriteBatch.begin();

	// Player texture
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static GTEngine::GLTexture texture = GTEngine::ResourceManager::getTexture("Textures/circle.png");

	GTEngine::ColorRGBA8 color;
	color.r = 255;
	color.b = 100;
	color.g = 155;
	color.a = 255;

	_spriteBatch.draw(_playerPosition, uv, texture.id, color, 0.0f);

	for (int i = 0; i < _bullets.size(); i++) {
		_bullets[i].draw(_spriteBatch);
	}

	for (int i = 0; i < _humans.size(); i++) {
		_humans[i].draw(_spriteBatch);
	}

	for (int i = 0; i < _zombies.size(); i++) {
		_zombies[i].draw(_spriteBatch);
	}


	_world.draw(_spriteBatch);

	_spriteBatch.end();

	_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);


	_textureProgram.unuse();
   
    // Swap our buffer and draw everything to the screen!
    _window.swapBuffer();
}

void MainGame::printFPS(int numberOfFrames, float fps) {
	// Print only once every 10 frames
	static int frameCounter = 0;
	++frameCounter;
	if (frameCounter == numberOfFrames) {
		std::cout << "FPS: " << fps << std::endl;
		frameCounter = 0;
	}
}

void MainGame::initCameraAndPlayer() {
	glm::vec4 position(_world.getPlayerStartPos().x * 50, _world.getPlayerStartPos().y * 50, 50.0f, 50.0f);
	_playerPosition = position;
	glm::vec2 cameraStartingPosition(_world.getPlayerStartPos().x * 50, _world.getPlayerStartPos().y * 50);
	_camera.setPosition(cameraStartingPosition);
}

void MainGame::initHumans() {
	srand(time(NULL));
	int humansCreated = 0;
	do {
		int rY = rand() % MAP_HEIGHT + 0;
		int rX = rand() % MAP_WIDTH + 1;


		if (_world.world[rX][rY] == 'R' || _world.world[rX][rY] == 'L' || _world.world[rX][rY] == 'G' ||
			_world.world[rX][rY] == 'B' || _world.world[rX][rY] == 'Z' || _world.world[rX][rY] == '@') {
			continue;
		}
		else {
			_humans.emplace_back(rX * 50, rY * 50);
			++humansCreated;
		}
	} while (humansCreated < _world.getHumanNumber());
}

void MainGame::initZombies() {
	int numZombies = 0;
	for (int i = 0; i < MAP_WIDTH; i++) {
		for (int j = 0; j < MAP_HEIGHT; j++) {
			if (_world.world[i][j] == 'Z') {
				++numZombies;
				_zombies.emplace_back(i * 50, j * 50);
			}
		}
	}
	_world.setHumanNumber(numZombies);
}

void MainGame::transformHumansToZombies(std::vector<Human>& humans, std::vector<Zombie>& zombies) {
	int bufferZone = 10;
	for (int i = 0; i < zombies.size(); i++) {
		for (int j = 0; j < humans.size(); j++) {
			float distance = sqrt(pow(humans[j].getPosition().x - zombies[i].getPosition().x, 2) + pow(humans[j].getPosition().y - zombies[i].getPosition().y, 2));
			if (distance <= bufferZone) { // Turn the human into a zombie
				zombies.emplace_back(humans[j].getPosition().x, humans[j].getPosition().y);
				humans[j] = humans.back();
				humans.pop_back();
			}
		}
	}
}