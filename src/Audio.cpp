#include "Audio.h"
#include <stdexcept>
#include <iostream>

Audio::Audio(FMOD::System* system, const std::string& filename)
    : system(system)
{
    if (system->createSound(filename.c_str(), FMOD_DEFAULT, nullptr, &sound) != FMOD_OK) {
        throw std::runtime_error("Failed to load sound: " + filename);
    }
    channel = nullptr;
}

Audio::~Audio() {
    if (sound) sound->release();
}

void Audio::play() {
    if (sound) {
        system->playSound(sound, nullptr, false, &channel);
    }
}

bool Audio::isPlaying() const {
    bool playing = false;
    if (channel) channel->isPlaying(&playing);
    return playing;
}
