#include "InputManager.h"

namespace GTEngine {

	InputManager::InputManager() : m_mouseCoords(0.0f)
	{
	}


	InputManager::~InputManager()
	{
	}

	void InputManager::update()
	{
		for (auto& it : m_keyMap) {
			m_previousKeyMap[it.first] = it.second;
		}
	}

	void InputManager::pressKey(unsigned int keyID) {
		m_keyMap[keyID] = true;
	}

	void InputManager::releaseKey(unsigned int keyID) {
		m_keyMap[keyID] = false;
	}


	void InputManager::setMouseCoords(float x, float y) {
		m_mouseCoords.x = x;
		m_mouseCoords.y = y;
	}

	bool InputManager::wasKeyDown(unsigned int keyID)
	{
		auto it = m_previousKeyMap.find(keyID);
		// Found the key
		if (it != m_previousKeyMap.end()) {
			return it->second;
		// Didn't find the key
		} else {
			return false;
		}
	}

	bool InputManager::isKeyPressed(unsigned int keyID)
	{
		if (isKeyDown(keyID) == true && wasKeyDown(keyID) == false)
			return true;
		else
			return false;
	}

	bool InputManager::isKeyDown(unsigned int keyID) {
		auto it = m_keyMap.find(keyID);
		// Found the key
		if (it != m_keyMap.end()) {
			return it->second;
		// Didn't find the key
		} else {
			return false;
		}
	}

}