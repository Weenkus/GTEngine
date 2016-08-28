#include "MainGame.h"
#include <GTEngine/ErrorHandler.h>
#include <GTEngine/ImageLoader.h>
#include <GTEngine/ResourceManager.h>

#include <iostream>
#include <string>

MainGame::MainGame() :
_screenHeight(768),
_screenWidth(1024),
_gameState(GameState::PLAY),
_time(0),
_maxFPS(60.0f)
{
	_camera.init(_screenWidth, _screenHeight);
}


MainGame::~MainGame()
{
}

void MainGame::run(){
	initSystem();

	//_playerTexture = ImageLoader::loadPNG("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

	gameLoop();
}

void MainGame::initSystem(){
	GTEngine::init();

	_window.create("Game Engine", _screenWidth, _screenHeight, 0);

	initShaders();

	_spriteBatch.init();
	_fpsLimiter.init(_maxFPS);
}

void MainGame::initShaders() {
	_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertexPosition");		// Vertex position in the vertex shader
	_colorProgram.addAttribute("vertexColor");			// Vertex color in the vertex shader
	_colorProgram.addAttribute("vertexUV");				// Texture coordinates in the shader
	_colorProgram.linkShaders();
}

void MainGame::gameLoop(){

	while (_gameState != GameState::EXIT) {
		_fpsLimiter.beginFrame();

		// Process users input
		processInput();
		_time += 0.01;

		// Update Game objects
		_camera.update();
		for (int i = 0; i < _bullets.size();) {
			if (_bullets[i].update() == true) {	// Erase the bullet 
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
			} else {
				i++;
			}
		}


		// Render game world
		drawGame();

		_fps = _fpsLimiter.endFrame();

		// Print only once every 10 frames
		static int frameCounter = 0;
		++frameCounter;
		if (frameCounter == 10000) {
			std::cout << _fps << std::endl;
			frameCounter = 0;
		}
	}
}

void MainGame::processInput(){
	SDL_Event evnt;

	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.1f;

	while (SDL_PollEvent(&evnt) == true) {
		// An event happened
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
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}

	if (_inputManager.isKeyDown(SDLK_w)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
	}
	if (_inputManager.isKeyDown(SDLK_s)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	}	
	if (_inputManager.isKeyDown(SDLK_a)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
	}	
	if (_inputManager.isKeyDown(SDLK_d)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
	}
	if (_inputManager.isKeyDown(SDLK_q)) {
		_camera.setScale(_camera.getScale() + SCALE_SPEED);
	}
	if (_inputManager.isKeyDown(SDLK_e)) {
		_camera.setScale(_camera.getScale() - SCALE_SPEED);
	}

	if (_inputManager.isKeyDown(SDL_BUTTON_LEFT)) {
		// Transfer window coordinates to world coordinates
		glm::vec2 mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = _camera.convertScreenToWorld(mouseCoords);
		std::cout << mouseCoords.x << " " << mouseCoords.y << std::endl;

		// Shoot bullets to mouse pointer
		glm::vec2 playerPosition(0.0f);
		glm::vec2 direction = mouseCoords - playerPosition;
		direction = glm::normalize(direction);

		_bullets.emplace_back(playerPosition, direction, 5.01f, 1000);
	}
}

void MainGame::drawGame() {

	// Set the base depth to 1.0
	glClearDepth(1.0);

	// Clear the depth and color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_colorProgram.use();
	glActiveTexture(GL_TEXTURE0);		// Use only one texture

	// Get the uniform location and send the uniform to the GPU
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);		// Use the above activated texture0

	// Set the ortho matrix
	GLint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();

	// Upload the matrix to the GPU
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	_spriteBatch.begin();

	glm::vec4 position(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static GTEngine::GLTexture texture = GTEngine::ResourceManager::getTexture("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	GTEngine::ColorRGBA8 color;

	color.r = 255;
	color.b = 255;
	color.g = 255;
	color.a = 255;

	_spriteBatch.draw(position, uv, texture.id, color, 0.0f);

	for (int i = 0; i < _bullets.size(); i++) {
		_bullets[i].draw(_spriteBatch);
	}

	_spriteBatch.end();

	_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);
	_colorProgram.unuse();

	// Swap buffers
	_window.swapBuffer();
}

