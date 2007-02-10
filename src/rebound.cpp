#include "rebound.hpp"

// SDL Includes
#include <SDL.h>
#include <SDL_image.h>

// STL Includes
#include <sstream>
#include <string>

// Rebound Includes
#include "exception.hpp"

// Bring into our namespace for ease of use
using std::string;
using std::stringstream;

namespace rebound {

  // These refer to actual space so they have to be 
  //  defined externally in the header
  const char* colorstr    = "^ORBYGC";
  const char* functionstr = "^WDKPB";
  const char* FONT_PATH   = "data/FreeSerifBold.ttf";  
  const char* IMAGE_DIR   = "./images08/";
  const char* PICTURE_DIR = "./pictures08/";
  const char* SOUND_DIR   = "./sounds/";

  // Loads an image from the given file and returns it as an
  //  sdl surface pointer
  SDL_Surface* loadImage(const string& filename, bool hasAlpha)
  { 
    // Temporary storage for the image that's loaded 
    SDL_Surface* loadedImage = 0;
    
    //The optimized image that will be used 
    SDL_Surface* optimizedImage = 0;
    
    // Load the image 
    loadedImage = IMG_Load( filename.c_str() );
    if (!loadedImage) {
      stringstream sstr;
      sstr << "Could not load image (" << filename << " ) :" 
	   << IMG_GetError();
      throw Exception(sstr.str());
    }
    
    // If nothing went wrong in loading the image 
    if(loadedImage) {
      // Create an optimized image 
      if (hasAlpha) 
	optimizedImage = SDL_DisplayFormatAlpha( loadedImage ); 
      else
	optimizedImage = SDL_DisplayFormat( loadedImage ); 
      
      //Free the old image 
      SDL_FreeSurface( loadedImage ); 
    }
    
    //Return the optimized image 
    return optimizedImage; 
  }
}

