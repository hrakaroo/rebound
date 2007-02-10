#include "painter.hpp"

#include <SDL.h>

#include "sdlrects.hpp"
#include "exception.hpp"
#include "rebound.hpp"

#include <iostream>

using std::cout;
using std::endl;

namespace rebound {

  // This function needs to be updated so that it is impossible
  //  to draw outside the _rect and to issue an update which is
  //  outside the rect.  For now we will just be careful, but this
  //  should be added for safety later.
  void 
  Painter::drawImage(SDL_Surface* image, 
		     const Rect& dest, 
		     const Rect& source) const
  {
    if (!image)
      throw Exception("Painter::drawImage: No image to draw");

    // The sdlDest will identify the x and y placement, the
    //  width and height are ignored.
    SDL_Rect sdlDest = dest.sdl(_rect);

    SDL_Rect updateRect;
    updateRect.x = (Sint16)(sdlDest.x);
    updateRect.y = (Sint16)(sdlDest.y);
    if (source.width() == 0)
      updateRect.w = image->w;
    else
      updateRect.w = (Uint16)(source.width()); 

    if (source.height() == 0)
      updateRect.h = image->h;
    else
      updateRect.h = (Uint16)(source.height());

    if (updateRect.x + updateRect.w > SCREEN_WIDTH) {
      cout << "adjusting" << endl;
      updateRect.w = SCREEN_WIDTH - updateRect.x;
    }
      
    if (updateRect.y + updateRect.h > SCREEN_HEIGHT) {
      cout << "adjusting" << endl;
      updateRect.h = SCREEN_HEIGHT - updateRect.y;
    }
    
    if (updateRect.w <= 0 or updateRect.h <= 0)
      throw Exception("Image is way off the page");

    _updateRects.add(updateRect);
    
    if (source.empty()) {
      // Then we need to copy the *entire* image
      SDL_BlitSurface(image, NULL, _screen, &sdlDest);
      
    } else {
      SDL_Rect sdlSource = source.sdl();
      SDL_BlitSurface(image, &sdlSource, _screen, &sdlDest);
    }
  }
}
