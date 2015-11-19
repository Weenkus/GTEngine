#pragma once

#include "Timing.h"

#include <SDL/SDL.h>

namespace GTEngine {

	FpsLimiter::FpsLimiter(){

	}

	void FpsLimiter::init(float maxFPS) {
		setTargetFPS(maxFPS);
	}

	void FpsLimiter::setTargetFPS(float maxFPS) {
		_maxFPS = maxFPS;
	}

	void FpsLimiter::beginFrame() {
		_startTicks = SDL_GetTicks();
	}

	// Return the current FPS
	float FpsLimiter::endFrame() {
		calculateFPS();

		float frameTicks = SDL_GetTicks() - _startTicks;
		// Limit the FPS to max FPS
		if (1000.0f / _maxFPS > frameTicks) {
			SDL_Delay((1000.0f / _maxFPS) - frameTicks);
		}

		return _fps;

	}

	void FpsLimiter::calculateFPS() {
		static const int NUM_SAMPLES = 100;
		static float frameTimes[NUM_SAMPLES];
		static int currentFrame = 0;

		static float prevTicks = SDL_GetTicks();

		float currentTicks;
		currentTicks = SDL_GetTicks();

		_frameTime = currentTicks - prevTicks;
		prevTicks = currentTicks;
		frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

		int count;

		// The frame circular buffer is not filled
		++currentFrame;
		if (currentFrame < NUM_SAMPLES) {
			count = currentFrame;
		}
		// The buffer is filled, set the count size to the sample size
		else {
			count = NUM_SAMPLES;
		}

		// Count is the number of frames we want to avergae
		float frameTimeAverga = 0;
		for (int i = 0; i < count; i++) {
			frameTimeAverga += frameTimes[i];
		}
		frameTimeAverga /= count;

		// Don't devide with zero!
		if (frameTimeAverga > 0) {
			_fps = 1000.0f / frameTimeAverga;
		}
		else {
			_fps = 60.0f;		// This should happen only for the first frame
		}
	}



}