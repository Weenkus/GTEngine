#pragma once

#include <GTEngine/Window.h>
#include <GTEngine/GLSLProgram.h>
#include <GTEngine/Camera2D.h>
#include <GTEngine/InputManager.h>
#include <GTEngine/Timing.h>
#include <GTEngine/Sprite.h>
#include <GTEngine/SpriteBatch.h>
#include <GTEngine/ResourceManager.h>

#include "Bullet.h"
#include "World.h"
#include "Human.h"
#include "Zombie.h"

enum class GameState { PLAY, EXIT };

class MainGame
{
public:
    MainGame();
    ~MainGame();

    /// Runs the game
    void run();

private:
    /// Initializes the core systems
    void initSystems();

    /// Initializes the shaders
    void initShaders();

    /// Main game loop for the program
    void gameLoop();

    /// Handles input processing
    void processInput();

    /// Renders the game
    void drawGame();

	/// Tell the users the frame rate of the game
	void printFPS(int numberOfFrames, float fps);

	/// Initialise the player and the camera
	void initCameraAndPlayer();

	/// Initialise all the humans
	void initHumans();

	/// Initialise all the zombies
	void initZombies();

	/// Zombies can infect humans
	void transformHumansToZombies(std::vector<Human>& humans, std::vector<Zombie>& zombies);

    /// Member Variables
	GTEngine::Window _window; ///< The game window
    
	GTEngine::GLSLProgram _textureProgram; ///< The shader program

	GTEngine::InputManager _inputManager; ///< Handles input

	GTEngine::Camera2D _camera; ///< Main Camera

	GTEngine::SpriteBatch _spriteBatch; ///< Sprite batch

	GTEngine::FpsLimiter _fpsLimiter;	///< FPS limiter

	World _world;	/// 2D world

	std::vector<Bullet> _bullets;
	std::vector<Human> _humans;
	std::vector<Zombie> _zombies;

	int _screenWidth;
	int _screenHeight;
	float _maxFPS;
	GameState _gameState;
	glm::vec4 _playerPosition;
};

