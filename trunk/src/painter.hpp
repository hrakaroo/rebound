#ifndef __PAINTER_HPP__
#define __PAINTER_HPP__

// Needed for class member
#include "rect.hpp"

// Forward declare the sdl structs for faster compile
struct SDL_Surface;

// Put us in the rebound namespace (duh)
namespace rebound {

  class SDLRects;

  // Painter class is used to make it easier to draw on an sdl
  // screen without needing to know what offsets are
  class Painter
  {
    SDL_Surface*           _screen;
    SDLRects&              _updateRects;
    Rect                   _rect;
    
  public:
    // Basic constructor with a target and placement
    Painter(SDL_Surface* screen, 
	    SDLRects& updateRects,
	    const Rect& rect = Rect())
      : _screen(screen), _updateRects(updateRects), _rect(rect) {}
    
    
    // Copy constructor
    Painter(const Painter& painter)
      : _screen(painter._screen), 
	_updateRects(painter._updateRects),
	_rect(painter._rect) {}

    // Create a child of this painter (on the same screen) but 
    //  with a different placement
    Painter area(const Rect& r) const { return Painter(_screen, _updateRects, r); }
    
    // Generic fetch methods
    inline int x() const { return _rect.x(); }
    inline int y() const { return _rect.y(); }
    inline int width() const { return _rect.width(); }
    inline int height() const { return _rect.height(); }
    
    // A request to draw a given image on the surface
    void drawImage(SDL_Surface* image, 
		   const Rect& dest = Rect(), 
		   const Rect& source = Rect()) const;

  private:
  };
}

#endif //__PAINTER_HPP__
