#pragma once

#include <SFML/Audio/Music.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <filesystem>
#include <map>

namespace music
{

enum class theme
{
    menu,
    mission
};

class player : private sf::NonCopyable
{
public:
    player();

    void play(
        theme const t);

    void stop();

    void pause(
        bool const paused);

    void volume(
        float const volume);

private:
    sf::Music music;
    std::map<theme, std::filesystem::path> filepaths;
    float volume_;
};

}
