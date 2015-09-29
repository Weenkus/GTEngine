#pragma once

#include <unordered_map>

#include <glm/glm.hpp>

namespace GTEngine {

	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void pressKey(unsigned int keyID);
		void releaseKey(unsigned int keyID);

		bool isKeyPressed(unsigned int keyID);

		// Setters
		void setMouseCoords(float x, float y);

		// getters
		glm::vec2 getMouseCoords() const { return _mouseCoords; }

	private:
		std::unordered_map<unsigned int, bool> _keyMap;
		glm::vec2 _mouseCoords;

	};

}