#pragma once

#include <GTEngine/Window.h>
#include <GTEngine/GLSLProgram.h>
#include <GTEngine/Camera2D.h>
#include <GTEngine/InputManager.h>
#include <GTEngine/Timing.h>
#include <GTEngine/Sprite.h>
#include <GTEngine/SpriteBatch.h>
#include <GTEngine/ResourceManager.h>
#include <GTEngine/SpriteFont.h>
#include <GTEngine/AudioEngine.h>
#include <GTEngine/ParticalBatch2D.h>
#include <GTEngine/ParticalEngine2D.h>

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

    void run();

private:
    void initSystems();
    void initShaders();
	void initCameraAndPlayer();
	void initHumans();
	void initZombies();

  
    void gameLoop();
    void processInput();


    void drawGame();
	void drawHud();

	void printFPS(int numberOfFrames, float fps);


	void transformHumansToZombies(std::vector<Human>& humans, std::vector<Zombie>& zombies);


	GTEngine::Window m_window;
    
	GTEngine::GLSLProgram m_textureProgram; 

	GTEngine::InputManager m_inputManager; 

	GTEngine::Camera2D m_camera; 
	GTEngine::Camera2D m_hudCamera;

	GTEngine::SpriteBatch m_spriteBatch; 
	GTEngine::SpriteBatch m_hudSpriteBatch;

	GTEngine::FpsLimiter m_fpsLimiter;	

	World m_world;	

	std::vector<Bullet> m_bullets;
	std::vector<Human> m_humans;
	std::vector<Zombie> m_zombies;

	int m_screenWidth{ 1024 };
	int m_screenHeight{ 768 };
	float m_maxFPS{ 200 };
	GameState m_gameState{ GameState::PLAY };
	glm::vec4 m_playerPosition;

	GTEngine::AudioEngine m_audioEngine;

	GTEngine::SpriteFont* m_spriteFont;

	GTEngine::ParticalEngine2D m_particalEngine;
	GTEngine::ParticalBatch2D* m_bloodParticalBatch;
};

