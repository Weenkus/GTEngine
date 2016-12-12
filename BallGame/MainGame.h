#pragma once

#include <GTEngine/Camera2D.h>
#include <GTEngine/SpriteBatch.h>
#include <GTEngine/InputManager.h>
#include <GTEngine/Window.h>
#include <GTEngine/GLSLProgram.h>
#include <GTEngine/Timing.h>
#include <GTEngine/SpriteFont.h>
#include <GTEngine/ParticalBatch2D.h>
#include <GTEngine/ParticalEngine2D.h>
#include <memory>

#include "BallController.h"
#include "BallRenderer.h"
#include "Grid.h"

// TODO:
// Visualize momentum with color
// Visualize velocity with color
// Visualize position with color

enum class GameState { RUNNING, EXIT };

const int CELL_SIZE = 24;

class MainGame {
public:
    ~MainGame();
    void run();


private:
    void init();
    void initRenderers();
    void initBalls();
    void update(float deltaTime);
    void draw();
    void drawHud();
    void processInput();

    int m_screenWidth = 0;
    int m_screenHeight = 0;

    std::vector<Ball> m_balls; ///< All the balls
    std::unique_ptr<Grid> m_grid; ///< Grid for spatial partitioning for collision

    int m_currentRenderer = 0;
    std::vector<std::unique_ptr<BallRenderer> > m_ballRenderers;

    BallController m_ballController; ///< Controls balls

	GTEngine::Window m_window; ///< The main window
	GTEngine::SpriteBatch m_spriteBatch; ///< Renders all the balls
    std::unique_ptr<GTEngine::SpriteFont> m_spriteFont; ///< For font rendering
	GTEngine::Camera2D m_camera; ///< Renders the scene
	GTEngine::InputManager m_inputManager; ///< Handles input
	GTEngine::GLSLProgram m_textureProgram; ///< Shader for textures]

	GTEngine::FpsLimiter m_fpsLimiter; ///< Limits and calculates fps
    float m_fps = 0.0f;

    GameState m_gameState = GameState::RUNNING; ///< The state of the game

	// Partical engine
	GTEngine::ParticalEngine2D m_particalEngine;
	GTEngine::ParticalBatch2D* m_collisionParticalBatch;
};

