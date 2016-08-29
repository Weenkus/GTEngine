#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <GTEngine/Sprite.h>
#include <GTEngine/GLSLProgram.h>
#include <GTEngine/GLTexture.h>
#include <GTEngine/Window.h>
#include <GTEngine/GTEngine.h>
#include <GTEngine/Camera2D.h>
#include <GTEngine/SpriteBatch.h>
#include <GTEngine/InputManager.h>
#include <GTEngine/Timing.h>

#include "Bullet.h"

#include <vector>

enum class GameState {PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();


private:
	void initSystem();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame();

	GTEngine::Window m_window;
	int m_screenWidth;
	int m_screenHeight;
	GameState m_gameState;

	GTEngine::GLSLProgram _colorProgram;
	GTEngine::Camera2D m_camera;

	GTEngine::SpriteBatch m_spriteBatch;

	GTEngine::InputManager m_inputManager;
	GTEngine::FpsLimiter m_fpsLimiter;

	std::vector<Bullet> m_bullets;

	float m_fps;
	float m_maxFPS;
	float m_time;
};

