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

	GTEngine::Window _window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;

	GTEngine::GLSLProgram _colorProgram;
	GTEngine::Camera2D _camera;

	GTEngine::SpriteBatch _spriteBatch;

	GTEngine::InputManager _inputManager;
	GTEngine::FpsLimiter _fpsLimiter;

	std::vector<Bullet> _bullets;

	float _fps;
	float _maxFPS;
	float _time;
};

