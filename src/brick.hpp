#ifndef __BRICK_HPP__
#define __BRICK_HPP__

// Ostream is needed so that we can debug print the brick
// We also use the string version for a key in the image manager
#include <ostream>

// Rebound app defaults
#include "rebound.hpp"

// Get us into the rebound namespace
namespace rebound {

  // Class to describe, but not draw, a brick on the screen
  class Brick
  {
    FUNCTION  _function;  // The function of the brick (key, paint, ...)
    COLOR     _color;     // The bricks color
    
  public:
    // Empty constructor
    Brick() : _function(EMPTY), _color(LTBLUE) {}

    // Constructor given just a function
    Brick(FUNCTION f) : _function(f), _color(LTBLUE) {}
    
    // Constructor given both a function and color
    Brick(FUNCTION f, COLOR c) : _function(f), _color(c) {}
    
    // Copy constructor
    Brick(const Brick& brick) : _function(brick._function),
				_color(brick._color) {}
    
    // Destructor
    ~Brick() {}
    
    // Basic get and fetch methods
    inline FUNCTION function() const { return _function; }
    inline void function(FUNCTION f) { _function = f; }

    inline COLOR color() const { return _color; }
    inline void color(COLOR c) { _color = c; }
  };
}
  
// Allows cout printing of the brick and allows us to stringify
//  the brick so we can use it as a key in the image manager.
std::ostream& operator<<(std::ostream& os, const rebound::Brick& brick);


#endif //__BRICK_HPP__
