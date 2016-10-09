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

MainGame::MainGame()
{
	m_camera.init(m_screenWidth, m_screenHeight);
	m_hudCamera.init(m_screenWidth, m_screenHeight);
}

MainGame::~MainGame() {
    // IMPLEMENT THIS!
}

void MainGame::run() {
	initSystems();

	GTEngine::Music music = m_audioEngine.loadMusic("Sound/XYZ.ogg");
	music.play(GTEngine::Music::LoopSetting::FOREVER);

	gameLoop();
}

void MainGame::initSystems() {
	GTEngine::init();
	m_audioEngine.init();

	m_window.create("Zombie Game0", m_screenWidth, m_screenHeight, 0);
	initShaders();

	// Initialise sprite batches
	m_spriteBatch.init();
	m_hudSpriteBatch.init();

	m_fpsLimiter.init(m_maxFPS);
	m_world.init();

	// Initialise SpriteFont
	m_spriteFont = new GTEngine::SpriteFont("Fonts/fast99.ttf", 32);

	// Init agents
	initCameraAndPlayer();
	initHumans();
	initZombies();

	// Play Music
	//PlaySound(TEXT("Sound/hoc.wav"), NULL, SND_ASYNC | SND_NOSTOP);
}

void MainGame::initShaders() {
    // Compile our color shader
    m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
    m_textureProgram.addAttribute("vertexPosition");
    m_textureProgram.addAttribute("vertexColor");
    m_textureProgram.addAttribute("vertexUV");
    m_textureProgram.linkShaders();
}

void MainGame::gameLoop() {
	while (m_gameState != GameState::EXIT) {
		m_fpsLimiter.beginFrame();

		// Process user input
		processInput();

		// Update the game state
		m_camera.update();
		m_hudCamera.update();
		
		for (int i = 0; i < m_bullets.size();) {
			if (m_bullets[i].update(m_world) == true) {	// Erase the bullet 
				m_bullets[i] = m_bullets.back();
				m_bullets.pop_back();
			}
			else {
				i++;
			}
		}
		srand(time(NULL));
		for (int i = 0; i < m_humans.size();) {
			if (m_humans[i].update(m_world, m_bullets) == true) { // Kill the human
				m_humans[i] = m_humans.back();
				m_humans.pop_back();
			}
			else {
				i++;
			}
		}
		for (int i = 0; i < m_zombies.size();) {
			if (m_zombies[i].update(m_world, m_bullets, m_humans) == true) { // Kill the human
				m_zombies[i] = m_zombies.back();
				m_zombies.pop_back();
			}
			else {
				i++;
			}
		}

		m_inputManager.update();
		
		transformHumansToZombies(m_humans, m_zombies);

		// Render the game
		drawGame();
		

		// Limit and print the FPS
		float fps = m_fpsLimiter.endFrame();
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
				// Play gunfire
				//PlaySound(TEXT("Sound/gun.mp3"), NULL, SND_ASYNC);
                break;
            case SDL_MOUSEBUTTONUP:
                m_inputManager.releaseKey(evnt.button.button);
                break;
        }
    }

	// Allow user movement
	int bufferSpace = 49;
	int minBufferSpace = 1;
	if (m_inputManager.isKeyDown(SDLK_w) && m_world.collision(m_playerPosition.x, m_playerPosition.y + bufferSpace)) {
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
		m_playerPosition.y += CAMERA_SPEED;
	}
	if (m_inputManager.isKeyDown(SDLK_s) && m_world.collision(m_playerPosition.x, m_playerPosition.y - minBufferSpace)) {
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
		m_playerPosition.y -= CAMERA_SPEED;
	}
	if (m_inputManager.isKeyDown(SDLK_a) && m_world.collision(m_playerPosition.x - minBufferSpace, m_playerPosition.y)) {
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
		m_playerPosition.x -= CAMERA_SPEED;
	}
	if (m_inputManager.isKeyDown(SDLK_d) && m_world.collision(m_playerPosition.x + bufferSpace, m_playerPosition.y)) {
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
		m_playerPosition.x += CAMERA_SPEED;
	}
	if (m_inputManager.isKeyDown(SDLK_q)) {
		m_camera.setScale(m_camera.getScale() + SCALE_SPEED);
	}
	if (m_inputManager.isKeyDown(SDLK_e)) {
		m_camera.setScale(m_camera.getScale() - SCALE_SPEED);
	}

	// Shooting bullets
	if (m_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
		GTEngine::SoundEffect effect = m_audioEngine.loadSoundEffect("Sound/shots/pistol.wav");
		effect.play();

		// Transfer window coordinates to world coordinates
		glm::vec2 mouseCoords = m_inputManager.getMouseCoords();
		mouseCoords = m_camera.convertScreenToWorld(mouseCoords);
		//std::cout << mouseCoords.x << " " << mouseCoords.y << std::endl;

		// Shoot bullets to mouse pointer
		glm::vec2 playerPosition(0.0f);
		playerPosition.x = m_playerPosition.x;
		playerPosition.y = m_playerPosition.y;

		glm::vec2 direction = mouseCoords - playerPosition;
		direction = glm::normalize(direction);

		m_bullets.emplace_back(1000, 5.01f, direction, playerPosition);
	}
}

void MainGame::drawGame() {
    // Set the base depth to 1.0
    glClearDepth(1.0);
    // Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // IMPLEMENT THIS!
	m_textureProgram.use();

	glActiveTexture(GL_TEXTURE0);		// Use only one texture

	// Set uniforms
	GLint textureLocation = m_textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);		// Use the above activated texture0

	GLint pLocation = m_textureProgram.getUniformLocation("P");		// Set the ortho matrix
	glm::mat4 cameraMatrix = m_camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));		// Upload the matrix to the GPU

	m_spriteBatch.begin();


	// Player texture
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static GTEngine::GLTexture texture = GTEngine::ResourceManager::getTexture("Textures/circle.png");

	GTEngine::ColorRGBA8 color(255, 100, 155, 255);

	const glm::vec2 agentDimensions(49);
	m_spriteBatch.draw(m_playerPosition, uv, texture.id, color, 0.0f);

	for (int i = 0; i < m_bullets.size(); i++) {
		m_bullets[i].draw(m_spriteBatch);
	}

	for (int i = 0; i < m_humans.size(); i++) {
		if(m_camera.isBoxInView(m_humans[i].getPosition(), agentDimensions))
			m_humans[i].draw(m_spriteBatch);
	}

	for (int i = 0; i < m_zombies.size(); i++) {
		if (m_camera.isBoxInView(m_zombies[i].getPosition(), agentDimensions))
			m_zombies[i].draw(m_spriteBatch);
	}
	m_world.draw(m_spriteBatch);

	

	m_spriteBatch.end();
	

	m_spriteBatch.renderBatch();

	drawHud();

	glBindTexture(GL_TEXTURE_2D, 0);


	m_textureProgram.unuse();
	
    // Swap our buffer and draw everything to the screen!
    m_window.swapBuffer();
}

void MainGame::drawHud()
{
	char buffer[256];

	glm::mat4 cameraMatrix = m_hudCamera.getCameraMatrix();
	GLint pLocation = m_textureProgram.getUniformLocation("P");		// Set the ortho matrix
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));		// Upload the matrix to the GPU

	m_hudSpriteBatch.begin();
	{
		sprintf_s(buffer, "Humans: %d", m_humans.size());
		m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(450, 260), glm::vec2(1.0), 0.0f,
			GTEngine::ColorRGBA8(255, 255, 255, 255), GTEngine::Justification::RIGHT);

		sprintf_s(buffer, "Num Zombies: %d", m_zombies.size());
		m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(450, 290), glm::vec2(1.0), 0.0f,
			GTEngine::ColorRGBA8(255, 255, 255, 255), GTEngine::Justification::RIGHT);

		sprintf_s(buffer, "Num Bullets: %d", m_bullets.size());
		m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(450, 320), glm::vec2(1.0), 0.0f,
			GTEngine::ColorRGBA8(255, 255, 255, 255), GTEngine::Justification::RIGHT);
	}
	m_hudSpriteBatch.end();
	m_hudSpriteBatch.renderBatch();
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
	glm::vec4 position(m_world.getPlayerStartPos().x * 50, m_world.getPlayerStartPos().y * 50, 50.0f, 50.0f);
	m_playerPosition = position;
	glm::vec2 cameraStartingPosition(m_world.getPlayerStartPos().x * 50, m_world.getPlayerStartPos().y * 50);
	m_camera.setPosition(cameraStartingPosition);
}

void MainGame::initHumans() {
	srand(time(NULL));
	int humansCreated = 0;
	do {
		int rY = rand() % MAP_HEIGHT + 0;
		int rX = rand() % MAP_WIDTH + 1;


		if (m_world.world[rX][rY] == 'R' || m_world.world[rX][rY] == 'L' || m_world.world[rX][rY] == 'G' ||
			m_world.world[rX][rY] == 'B' || m_world.world[rX][rY] == 'Z' || m_world.world[rX][rY] == '@') {
			continue;
		}
		else {
			m_humans.emplace_back(rX * 50, rY * 50);
			++humansCreated;
		}
	} while (humansCreated < m_world.getHumanNumber());
}

void MainGame::initZombies() {
	int numZombies = 0;
	for (int i = 0; i < MAP_WIDTH; i++) {
		for (int j = 0; j < MAP_HEIGHT; j++) {
			if (m_world.world[i][j] == 'Z') {
				++numZombies;
				m_zombies.emplace_back(i * 50, j * 50);
			}
		}
	}
	m_world.setHumanNumber(numZombies);
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