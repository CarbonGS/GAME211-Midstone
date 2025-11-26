#include "Audio.h"
#include <stdexcept>
#include <iostream>
#include <SDL3/SDL.h>

Audio::Audio(const std::string& filename)
{
	SDL_LoadWAV(filename.c_str(), &wavSpec, &wavBuffer, &wavLength);


	SDL_AudioStream* stream = SDL_OpenAudioDeviceStream(
		SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
		&wavSpec,
		nullptr,
		nullptr
	);

	if (!stream) {
		SDL_free(wavBuffer);
		throw std::runtime_error("Failed to open audio stream: " + std::string(SDL_GetError()));

	}

	if (!SDL_PutAudioStreamData(stream, wavBuffer, wavLength)) {
		SDL_free(wavBuffer);
		SDL_DestroyAudioStream(stream);
		throw std::runtime_error("Failed to put WAV data into stream: " + std::string(SDL_GetError()));
	}

	// SDL_free(wavBuffer);

	this->audioStream = stream;
}

Audio::~Audio() {
	if (audioStream) {
		SDL_DestroyAudioStream(audioStream);
	}
}

void Audio::play() {
	if (audioStream) {
		SDL_ResumeAudioStreamDevice(audioStream);
	}
}

bool Audio::isPlaying() const {
	if (!audioStream) return false;
	int queued = SDL_GetAudioStreamQueued(audioStream);
	return (queued > 0);
}
