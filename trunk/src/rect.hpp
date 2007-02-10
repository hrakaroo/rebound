#ifndef __RECT_HPP__
#define __RECT_HPP__

#include <ostream>

#include <SDL.h>

namespace rebound {
  
  class Rect
  {
    int _x;
    int _y;
    int _width;
    int _height;
    
  public:
    Rect() 
      : _x(0), _y(0), _width(0), _height(0) {}
    
    Rect(int x, int y, int width, int height) 
      : _x(x), _y(y), _width(width), _height(height) {}
    
    inline int x() const { return _x; }
    inline int y() const { return _y; }
    inline int width() const { return _width; }
    inline int height() const { return _height; }
    
    inline void x(int x_) { _x = x_; }
    inline void y(int y_) { _y = y_; }
    inline void width(int width_) { _width = width_; }
    inline void height(int height_) { _height = height_; }
    
    inline bool empty() const
    { return (_x == 0 && _y == 0 && _width == 0 && _height == 0); }
    
    SDL_Rect sdl(const Rect& rect = Rect()) const;
    
  };
}
  
std::ostream& operator<<(std::ostream& os, const rebound::Rect& r);


#endif //__RECT_HPP__
