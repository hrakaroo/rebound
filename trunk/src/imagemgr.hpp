#ifndef __IMAGEMGR_HPP__
#define __IMAGEMGR_HPP__

/*
  The Image Manager enables for Images to be loaded as early 
  as possible so we know about any missing images before starting
  the actual game.  It also enables us to load the images in one
  place, but use them anywhere without having to worry about who
  owns the images, or that we might be wasting resources and time
  by loading an image more than once.
 */

// Map is needed for lookup table of image pointers
#include <map>
#include <string>

// Sdl surface is needed as pointer so forward declare it here
struct SDL_Surface;

// Namespace for the game
namespace rebound {

  // Forward declare so we don't need to bring in the full header
  class Ball;
  class Brick;

  // Typedef for ease of use and to help the compiler not get confused
  typedef std::map<std::string, SDL_Surface*> ImageMap;

  // The application only makes one of these
  class ImageMgr
  {
    // Main map to store the image pointers
    ImageMap    _images;
    std::string _directory;
    
  public:
    // Empty constructor
    ImageMgr(const std::string& directory = "") : _directory(directory) {};

    // Destructor must clean up image pointers
    ~ImageMgr();

    // Add images
    void load(const std::string& tag, const std::string& file, 
	      bool alpha=false);

    void load(const Ball& ball,       const std::string& file, 
	      bool alpha=false);

    void load(const Brick& brick,     const std::string& file, 
	      bool alpha=false);
    
    // Fetch images
    SDL_Surface* fetch(const std::string& tag) const;
    SDL_Surface* fetch(const Ball& ball) const;
    SDL_Surface* fetch(const Brick& brick) const;
    
  private:
    // Disallow copying (it would cause us to delete the pointer twice
    ImageMgr(const ImageMgr&) {}
  };
}

#endif //__IMAGEMGR_HPP__
