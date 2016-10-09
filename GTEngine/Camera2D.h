#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace GTEngine {

	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();

		void init(int screenWidth, int screenHeight);
		void update();

		glm::vec2 convertScreenToWorld(glm::vec2 screenCoords);
		inline double invertY(double y) const { return m_screenHeight - y; }

		bool isBoxInView(const glm::vec2& position, const glm::vec2& dimensions);

		void setPosition(const glm::vec2& newPosition) { m_position = newPosition; m_needsMatrixUpdate = true; }
		void setScale(float newScale) { m_scale = newScale; m_needsMatrixUpdate = true; }

		glm::mat4 getCameraMatrix() const { return m_cameraMatrix; }
		glm::vec2 getPosition() const { return m_position; }
		glm::vec2 getCentarScreenPosition() const { return glm::vec2(m_screenWidth / 2, m_screenHeight / 2); }
		float getScale() const { return m_scale; }
		

	private:
		void translate();
		void scale();

		int m_screenWidth{ 500 }, m_screenHeight{ 500 };
		bool m_needsMatrixUpdate{ true };
		float m_scale{ 1.0f };
		glm::vec2 m_position{ 0.0f, 0.0f };
		glm::mat4 m_cameraMatrix{ 1.0f };
		glm::mat4 m_orthoMatrix{ 1.0f };
	};

}
