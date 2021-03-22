#include "music.h"

#include <exception>

namespace music
{

player::player()
    : volume_{100.f}
{
    filepaths[theme::menu] = "Media/Music/MenuTheme.ogg";
    filepaths[theme::mission] = "Media/Music/MissionTheme.ogg";
}

void player::play(
    theme const t)
{
    if(!music.openFromFile(filepaths[t]))
    {
        throw std::runtime_error("Music " + filepaths[t].string() + " could not be loaded.");
    }

    music.setVolume(volume_);
    music.setLoop(true);
    music.play();
}

void player::stop()
{
    music.stop();
}

void player::pause(
    bool const paused)
{
    if(paused)
    {
        music.pause();
    }
    else
    {
        music.play();
    }
}

void player::volume(
    float const volume)
{
    music.setVolume(volume_ = volume);
}

}
