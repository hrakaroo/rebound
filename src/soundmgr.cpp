// Our header
#include "soundmgr.hpp"

// STL Includes
#include <sstream>
#include <string>

// Rebound Includes
#include "exception.hpp"

// Ease of use
using std::string;
using std::stringstream;

namespace rebound {

  SoundMgr::~SoundMgr() 
  {
    // Clean up all the sounds we have been managing
    for (SoundMap::const_iterator i = _sounds.begin();
	 i != _sounds.end(); ++i) {
      Mix_FreeChunk(i->second);
    }
  }

  void
  SoundMgr::load(const string& tag, const string& path)
  {
    Mix_Chunk* sound = Mix_LoadWAV((_directory + "/" + path).c_str());
    if (!sound) {
      stringstream sstr;
      sstr << "Mix_LoadWAV could not load sound (" << path << ")" 
	   << Mix_GetError();
      throw Exception(sstr.str());
    }
    _sounds.insert(std::make_pair(tag, sound));
  }


  void 
  SoundMgr::play(const string& tag, int channel) const
  {
    // Try to find it first
    SoundMap::const_iterator it = _sounds.find(tag);
    if (it == _sounds.end()) 
      throw Exception("Could not find sound: " + tag);
      
    // Play the sound
    if (Mix_PlayChannel(channel, it->second, 0) == -1) {
      // This may be caused by no free sounds channels available.
      //  We need to check for that specific instance or else throw
      //  For now, ignore if the sound does not play
      //stringstream sstr;
      //sstr << "Mix_PlayChannel could not play sound: " 
      //<< Mix_GetError();
      //throw Exception(sstr.str());
    }
  }
}
