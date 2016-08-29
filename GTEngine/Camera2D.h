#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace GTEngine {

	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();

		// Sets up the orthographic matrix and screen dimensions
		void init(int screenWidth, int screenHeight);

		// Updates the camera matrix if needed
		void update();

		glm::vec2 convertScreenToWorld(glm::vec2 screenCoords);

		bool isBoxInView(const glm::vec2& position, const glm::vec2& dimensions);

		// Setters
		void setPosition(const glm::vec2& newPosition) { m_position = newPosition; m_needsMatrixUpdate = true; }
		void setScale(float newScale) { m_scale = newScale; m_needsMatrixUpdate = true; }

		// Getters
		glm::vec2 getPosition() const { return m_position; }
		float getScale() const { return m_scale; }
		glm::mat4 getCameraMatrix() const { return m_cameraMatrix; }

	private:
		int m_screenWidth, _screenHeight;
		bool m_needsMatrixUpdate;
		float m_scale;
		glm::vec2 m_position;
		glm::mat4 m_cameraMatrix;
		glm::mat4 m_orthoMatrix;
	};

}
