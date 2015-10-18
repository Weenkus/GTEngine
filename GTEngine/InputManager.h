#pragma once

#include <unordered_map>

#include <glm/glm.hpp>

namespace GTEngine {

	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void update();

		void pressKey(unsigned int keyID);
		void releaseKey(unsigned int keyID);

		// Returns true if the key was just presed (we need to check was it pressed the freme before)
		bool isKeyPressed(unsigned int keyID);

		// Returns true if the key is held down
		bool isKeyDown(unsigned int keyID);

		// Setters
		void setMouseCoords(float x, float y);

		// getters
		glm::vec2 getMouseCoords() const { return _mouseCoords; }

	private:
		bool wasKeyDown(unsigned int keyID);

		std::unordered_map<unsigned int, bool> _keyMap;
		std::unordered_map<unsigned int, bool> _previousKeyMap;
		glm::vec2 _mouseCoords;

	};

}