#include "MainGame.h"
#include <GTEngine/ErrorHandler.h>
#include <GTEngine/ImageLoader.h>
#include <GTEngine/ResourceManager.h>

#include <iostream>
#include <string>

MainGame::MainGame() :
m_screenHeight(768),
m_screenWidth(1024),
m_gameState(GameState::PLAY),
m_time(0),
m_maxFPS(60.0f)
{
	m_camera.init(m_screenWidth, m_screenHeight);
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

	m_window.create("Game Engine", m_screenWidth, m_screenHeight, 0);

	initShaders();

	m_spriteBatch.init();
	m_fpsLimiter.init(m_maxFPS);
}

void MainGame::initShaders() {
	_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertexPosition");		// Vertex position in the vertex shader
	_colorProgram.addAttribute("vertexColor");			// Vertex color in the vertex shader
	_colorProgram.addAttribute("vertexUV");				// Texture coordinates in the shader
	_colorProgram.linkShaders();
}

void MainGame::gameLoop(){

	while (m_gameState != GameState::EXIT) {
		m_fpsLimiter.beginFrame();

		// Process users input
		processInput();
		m_time += 0.01;

		// Update Game objects
		m_camera.update();
		for (int i = 0; i < m_bullets.size();) {
			if (m_bullets[i].update() == true) {	// Erase the bullet 
				m_bullets[i] = m_bullets.back();
				m_bullets.pop_back();
			} else {
				i++;
			}
		}


		// Render game world
		drawGame();

		m_fps = m_fpsLimiter.endFrame();

		// Print only once every 10 frames
		static int frameCounter = 0;
		++frameCounter;
		if (frameCounter == 10000) {
			std::cout << m_fps << std::endl;
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
			m_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			m_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			m_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			m_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}

	if (m_inputManager.isKeyDown(SDLK_w)) {
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
	}
	if (m_inputManager.isKeyDown(SDLK_s)) {
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	}	
	if (m_inputManager.isKeyDown(SDLK_a)) {
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
	}	
	if (m_inputManager.isKeyDown(SDLK_d)) {
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
	}
	if (m_inputManager.isKeyDown(SDLK_q)) {
		m_camera.setScale(m_camera.getScale() + SCALE_SPEED);
	}
	if (m_inputManager.isKeyDown(SDLK_e)) {
		m_camera.setScale(m_camera.getScale() - SCALE_SPEED);
	}

	if (m_inputManager.isKeyDown(SDL_BUTTON_LEFT)) {
		// Transfer window coordinates to world coordinates
		glm::vec2 mouseCoords = m_inputManager.getMouseCoords();
		mouseCoords = m_camera.convertScreenToWorld(mouseCoords);
		std::cout << mouseCoords.x << " " << mouseCoords.y << std::endl;

		// Shoot bullets to mouse pointer
		glm::vec2 playerPosition(0.0f);
		glm::vec2 direction = mouseCoords - playerPosition;
		direction = glm::normalize(direction);

		m_bullets.emplace_back(playerPosition, direction, 5.01f, 1000);
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
	glm::mat4 cameraMatrix = m_camera.getCameraMatrix();

	// Upload the matrix to the GPU
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	m_spriteBatch.begin();

	glm::vec4 position(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static GTEngine::GLTexture texture = GTEngine::ResourceManager::getTexture("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	GTEngine::ColorRGBA8 color;

	color.r = 255;
	color.b = 255;
	color.g = 255;
	color.a = 255;

	m_spriteBatch.draw(position, uv, texture.id, color, 0.0f);

	for (int i = 0; i < m_bullets.size(); i++) {
		m_bullets[i].draw(m_spriteBatch);
	}

	m_spriteBatch.end();

	m_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);
	_colorProgram.unuse();

	// Swap buffers
	m_window.swapBuffer();
}

