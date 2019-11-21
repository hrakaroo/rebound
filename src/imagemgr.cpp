// Our header
#include "imagemgr.hpp"

// Needed for stringifying the ball and brick
#include <sstream>
#include <string>

// Bring in the ball and brick for real now
#include "ball.hpp"
#include "brick.hpp"
#include "exception.hpp"

// Ease of use
using std::string;
using std::stringstream;

// Put us in our namespace
namespace rebound {
  
  // Delete all saved images
  ImageMgr::~ImageMgr() 
  {
    // Clean up all the images we have been managing
    for (ImageMap::const_iterator i = _images.begin();
	 i != _images.end(); ++i) {
      SDL_FreeSurface(i->second);
    }
    _images.clear();
  }
  
  // Core function to load an image
  void 
  ImageMgr::load(const string& tag, const string& file, bool alpha)
  {
    // Load the image
    SDL_Surface* image = loadImage(_directory + "/" + file, alpha);

    // Stick the new image into our stack
    _images.insert(std::make_pair(tag,image));
  }


  // Load the image as a ball, stringify and call core
  void 
  ImageMgr::load(const Ball& ball, const string& file, bool alpha)
  {
    // Stringify the ball
    stringstream sstr;
    sstr << ball;
    load(sstr.str(), file, alpha);
  }
  

  // Load the image as a brick, stringify and call core
  void 
  ImageMgr::load(const Brick& brick, const string& file, bool alpha)
  {
    // Stringify the brick
    stringstream sstr;
    sstr << brick;
    load(sstr.str(), file, alpha);
  }


  SDL_Surface* 
  ImageMgr::fetch(const string& tag) const
  {
    // Try to find it first
    ImageMap::const_iterator it = _images.find(tag);
    
    if (it == _images.end()) {
      throw Exception("Could not find image: " + tag);
    }

    return it->second;
  }
  
  SDL_Surface* 
  ImageMgr::fetch(const Ball& ball) const
  {
    stringstream sstr;
    sstr << ball;
    return fetch(sstr.str());
  }
  
  SDL_Surface* 
  ImageMgr::fetch(const Brick& brick) const
  {
    stringstream sstr;
    sstr << brick;
    return fetch(sstr.str());
  }
}  
