#include "sdlrects.hpp"

// SDL Includes
#include <SDL.h>

// Rebound Includes
#include "rebound.hpp"
#include "exception.hpp"

namespace rebound {

  // Constructor
  SDLRects::SDLRects() : _rects(0), _maxRects(MAXRECTS), _numRects(0) 
  {
    // We need to use malloc as we may need realloc later
    _rects = (SDL_Rect*)(malloc(sizeof(SDL_Rect)*_maxRects));
    if (!_rects) 
      throw Exception("Could not malloc rects");
  }

  // Destructor
  SDLRects::~SDLRects()
  {
    if (_rects) free(_rects);
  }

  void 
  SDLRects::add(const SDL_Rect& rect)
  {
    // Check if we have space for this one
    if (_numRects >= _maxRects) {
      // Double the list space 
      _maxRects *= 2;
      _rects = (SDL_Rect*)(realloc(_rects, sizeof(SDL_Rect)*_maxRects));
      if (!_rects)
	// This is really bad ... we ran out of memory
	throw Exception("Could not realloc rects");
    }

    // Stick it on the list
    _rects[_numRects++] = rect;
  }
  
}
