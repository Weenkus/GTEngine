#include "AudioEngine.h"
#include "ErrorHandler.h"

namespace GTEngine {

	void SoundEffect::play(int loops) {
		const int defaultChannel{ -1 };
		if (Mix_PlayChannel(defaultChannel, m_chunk, loops) == -1) {
			fatalError("Mix_PlayChannel error:" + std::string(Mix_GetError()));
		}
	}

	void Music::play(int loops) {
		if (Mix_PlayMusic(m_music, loops) == -1) {
			fatalError("Mix_PlayChannel error:" + std::string(Mix_GetError()));
		}
	}

	void Music::pause() {
		Mix_PauseMusic();
	}

	void Music::stop() {
		Mix_HaltMusic();
	}

	void Music::resume() {
		Mix_ResumeMusic();
	}

	AudioEngine::AudioEngine() {
		// Empty
	}


	AudioEngine::~AudioEngine()
	{
		destroy();
	}
	
	void AudioEngine::init() {
		// Parameter can be a bitwise combination of MIX_INIT_FAC, MIX_INIT_MOD, MIX_INIT_MP3, MIX_INIT_OGG
		if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1) {
			fatalError("Mix_Init error:" + std::string(Mix_GetError()));
		}

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, Channel::STEREO, ChuckSize::DEFAULT) == -1) {
			fatalError("Mix_OpenAudio error:" + std::string(Mix_GetError()));
		}

		this->m_isInitialized = true;
	}

	void AudioEngine::destroy() {
		if (m_isInitialized) {
			m_isInitialized = false;
			Mix_Quit();
		}
	}

	SoundEffect AudioEngine::loadSoundEffect(const std::string& filePath) {
		auto it = m_effectCache.find(filePath);
		SoundEffect effect;

		// Fail to find cache
		if (it == m_effectCache.end()) {
			Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
			if (chunk == nullptr) {
				fatalError("Mix_LoadWAV error:" + std::string(Mix_GetError()));
			}
			effect.m_chunk = chunk;
			m_effectCache[filePath] = chunk;
		// Found cache
		} else {
			effect.m_chunk = it->second;
		}

		return effect;
	}

	Music AudioEngine::loadMusic(const std::string& filePath) {
		auto it = m_musicCache.find(filePath);
		Music music;

		// Fail to find cache
		if (it == m_musicCache.end()) {
			Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());
			if (mixMusic == nullptr) {
				fatalError("Mix_LoadMUS error:" + std::string(Mix_GetError()));
			}
			music.m_music = mixMusic;
			m_musicCache[filePath] = mixMusic;
			// Found cache
		}
		else {
			music.m_music = it->second;
		}

		return music;
	}
}
