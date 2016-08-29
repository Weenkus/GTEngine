#include "Camera2D.h"

namespace GTEngine {

	inline bool is_collision(float xDepth, float yDepth);

	Camera2D::Camera2D()
	{
	}


	Camera2D::~Camera2D()
	{
	}

	void Camera2D::init(int screenWidth, int screenHeight) {
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;
		m_orthoMatrix = glm::ortho(0.0f, (float)m_screenWidth, 0.0f, (float)m_screenHeight);
	}

	void Camera2D::update() {
		if (m_needsMatrixUpdate) {
			translate();
			scale();
			m_needsMatrixUpdate = false;
		}
	}

	void Camera2D::translate() {
		glm::vec3 translate(-m_position.x + m_screenWidth / 2, -m_position.y + m_screenHeight / 2, 0.0f);
		m_cameraMatrix = glm::translate(m_orthoMatrix, translate);
	}

	void Camera2D::scale() {
		glm::vec3 scale(m_scale, m_scale, 0.0f);
		m_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * m_cameraMatrix;
	}

	glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 screenCoords) {
		screenCoords.y = invertY(screenCoords.y);
		screenCoords -= getCentarScreenPosition();
		screenCoords /= m_scale;
		screenCoords += m_position;
		
		return screenCoords;
	}


	bool Camera2D::isBoxInView(const glm::vec2 & position, const glm::vec2 & dimensions)
	{
		glm::vec2 scaledScreenDimensions = glm::vec2(m_screenWidth, m_screenHeight) / m_scale;

		// The minimum distance before a collision occures
		const float MIN_DISTANCE_X = dimensions.x / 2.0f + scaledScreenDimensions.x / 2.0f;
		const float MIN_DISTANCE_Y = dimensions.y / 2.0f + scaledScreenDimensions.y / 2.0f;

		glm::vec2 centerPositionOfBox = position + dimensions / 2.0f;
		glm::vec2 centerCameraPosition = m_position;
		glm::vec2 distance = centerPositionOfBox - centerCameraPosition;

		float xDepth = MIN_DISTANCE_X - abs(distance.x);
		float yDepth = MIN_DISTANCE_Y - abs(distance.y);

		return is_collision(xDepth, yDepth);
	}

	inline bool is_collision(float xDepth, float yDepth) {
		return xDepth > 0 && yDepth > 0;
	}

}