#include "rect.hpp"

namespace rebound {

  SDL_Rect 
  Rect::sdl(const Rect& rect) const
  { 
    SDL_Rect r; 
    r.x = (Sint16)_x + rect.x(); 
    r.y = (Sint16)_y + rect.y(); 
    r.w = (Uint16)_width; 
    r.h = (Uint16)_height; 
    return r;
  }
}


std::ostream& operator<<(std::ostream& os, const rebound::Rect& r)
{
  os << "Rect(" << r.x() << ", " << r.y() << ", " 
     << r.width() << ", " << r.height() << ")";
  return os;
}
