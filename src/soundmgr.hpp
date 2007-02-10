#ifndef __SOUNDMGR_HPP__
#define __SOUNDMGR_HPP__

/*
  The Sound Manager enables for Sounds to be loaded as early 
  as possible so we know about any missing sounds before starting
  the actual game.  It also enables us to load the sounds in one
  place, but use them anywhere without having to worry about who
  owns the sounds, or that we might be wasting resources and time
  by loading an sound more than once.

  Most of the design of this was taken from the ImageMgr
 */

// Map is needed for lookup table of sound pointers
#include <map>
#include <string>

// We can't do forward declare here so bring in all the SDL_Mixer
#include <SDL_mixer.h>

// Namespace for the game
namespace rebound {

  // Typedef for ease of use and to help the compiler not get confused
  typedef std::map<std::string, Mix_Chunk*> SoundMap;

  // The application only makes one of these
  class SoundMgr
  {
    // Main map to store the sound pointers
    SoundMap    _sounds;
    std::string _directory;
    
  public:
    // Empty constructor
    SoundMgr(const std::string& directory) : _directory(directory) {};

    // Destructor must clean up sound pointers
    ~SoundMgr();
    
    // Load a sound
    void load(const std::string& tag, const std::string& path);
    
    // Play a sound on a given channel, -1 asks for first free channel
    void play(const std::string& tag, int channel=-1) const;
    
  private:
    // Disallow copying (it would cause us to delete the pointer twice
    SoundMgr(const SoundMgr&) {}
  };
}

#endif //__SOUNDMGR_HPP__
