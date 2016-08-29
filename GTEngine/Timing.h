namespace GTEngine {

	class FpsLimiter {

	public:
		FpsLimiter();

		void init(float maxFPS);
		void setTargetFPS(float maxFPS);
		void beginFrame();

		// Return the current FPS
		float endFrame();

	private:
		void calculateFPS();

		float m_fps;
		float m_frameTime;
		float m_maxFPS;

		unsigned int m_startTicks;
	};

}