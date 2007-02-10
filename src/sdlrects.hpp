#ifndef __SDLRECTS_H__
#define __SDLRECTS_H__

/*
  SDL is a C library, so when we call UpdateRects it wants to take the
  rect list as a c pointer to an array.  (In C++ a vector of rects would
  probably be used).  The SDLRects class is intended to make interacting
  with the C pointer array more C++ like.  The internal data storage is
  a c pointer array as we don't want to waste time copying it out of a
  vector.

  malloc and realloc are used to get the memory for the array.
 */

struct SDL_Rect;

namespace rebound {
  
  class SDLRects
  {
    SDL_Rect* _rects;     // C pointer to array or SDL_Rects
    int       _maxRects;  // How many rects can we hold
    int       _numRects;  // How many rects *are* we holding
    
  public:
    // Basic constructor (mallocs SDL_Rect memory)
    SDLRects();
    
    // Destructor (frees up SDL_Rect memory)
    ~SDLRects();
    
    // Returns how many rects we have
    inline int numRects() const { return _numRects; }

    // Returns the c pointer
    inline SDL_Rect* rects() const { return _rects; }
    
    // Sets the count for the array to zero
    inline void reset() { _numRects = 0; }

    // Adds a rect to the SDL_Rects array, realloc if necessary
    void add(const SDL_Rect& rect);

  private:
    // Nopy copying allowed
    SDLRects(const SDLRects&) {}
  };
}

#endif //__SDLRECTS_H__
