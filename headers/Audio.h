#pragma once
#include <SDL3\SDL.h>
#include <string>

class Audio {


public:
	struct AudioData {
		Uint8* pos;
		Uint32 length;
	};

	Audio(const std::string& filename);

	~Audio();

	void play();
	bool isPlaying() const;

private:
	SDL_AudioSpec wavSpec;
	Uint8* wavBuffer = nullptr;
	Uint32 wavLength = 0;
	SDL_AudioDeviceID dev;
	SDL_AudioStream* audioStream = nullptr;

	AudioData audioData;


};
