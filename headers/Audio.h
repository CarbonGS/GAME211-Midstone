#pragma once
#include <fmod.hpp>
#include <string>

class Audio {
public:
    Audio(FMOD::System* system, const std::string& filename);
    ~Audio();
    void play();
    bool isPlaying() const;
private:
    FMOD::System* system = nullptr;
    FMOD::Sound* sound = nullptr;
    FMOD::Channel* channel = nullptr;
};
