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

		float _fps;
		float _frameTime;
		float _maxFPS;

		unsigned int _startTicks;
	};

}