#pragma once

#include <SDL/SDL_mixer.h>

#include <string>
#include <map>

namespace GTEngine {

	class SoundEffect {
	public:
		friend class AudioEngine;
		const enum LoopSetting { FOREVER = -1, ONCE = 0, TWICE = 2 };

		void play(int loops = LoopSetting::ONCE);

	private:
		Mix_Chunk* m_chunk = nullptr;
	};

	class Music {
	public:
		friend class AudioEngine;
		const enum LoopSetting { FOREVER = -1, ONCE = 1, TWICE = 2 };

		void play(int loops = LoopSetting::ONCE);

		static void pause();
		static void stop();
		static void resume();

	private:
		Mix_Music* m_music = nullptr;
	};

	class AudioEngine
	{
	public:
		AudioEngine();
		~AudioEngine();

		const enum Channel { MONO = 1, STEREO = 2};
		const enum ChuckSize { DEFAULT = 1024};
		

		void init();
		void destroy();

		SoundEffect loadSoundEffect(const std::string& filePath);
		Music loadMusic(const std::string& filePath);

	private:
		std::map<std::string, Mix_Chunk*> m_effectCache;
		std::map<std::string, Mix_Music*> m_musicCache;

		bool m_isInitialized = { false };
	};


}
