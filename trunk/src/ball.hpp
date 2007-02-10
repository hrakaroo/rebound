#ifndef __BALL_HPP__
#define __BALL_HPP__

// Ostream is needed so that we can debug print the ball
// We also use the string version for a key in the image manager
#include <ostream>

// A Ball has a rect and needs the rebound app defaults
#include "rect.hpp"
#include "rebound.hpp"

// Get us into the rebound namespace
namespace rebound {
  
  // Ball class which describes, but does not draw, a ball on the screen
  class Ball
  {
    COLOR     _color;     // Current color of the ball
    DIRECTION _direction; // Current direction the ball is bouncing
    int       _radius;    // Current radius of the ball (could be static)
    int       _x;         // Current x location on the screen
    int       _y;         // Current y location on the screen
    COLOR     _key;       // Current color of the key it is holding
    
  public:
    // Empty constructor with default values
    Ball() : _color(LTBLUE),
	     _direction(UP),
	     _radius(ballRadius),
	     _x(_radius+_radius),
	     _y(_radius+_radius),
	     _key(LTBLUE) {}
    
    // Constructor given a starting color
    Ball(COLOR c) : _color(c),
		    _direction(UP),
		    _radius(ballRadius),
		    _x(_radius+_radius),
		    _y(_radius+_radius),
		    _key(LTBLUE) {}
    
    // Copy constructor
    Ball(const Ball& ball) : _color(ball._color),
			     _direction(ball._direction),
			     _radius(ball._radius),
			     _x(ball._x),
			     _y(ball._y),
			     _key(ball._key) {}
    
    // Destructor
    ~Ball() {}
    
    // Basic fetch and set methods
    inline DIRECTION direction() const { return _direction; }
    inline void direction(DIRECTION d) { _direction = d; }
    
    inline COLOR color() const { return _color; }
    inline void color(COLOR c) { _color = c; }

    inline COLOR key() const { return _key; }
    inline void key(COLOR c) { _key = c; }
    
    inline int x() const { return _x; }
    inline void x(int xx) { _x = xx; }
    
    inline int y() const { return _y; }
    inline void y(int yy) { _y = yy; }
    
    inline void position(int x, int y) { _x = x; _y = y; }
    
    inline int radius() const {return _radius; }
    
    inline Rect rect() { return Rect(_x - _radius, _y - _radius,
				     _radius + _radius, 
				     _radius + _radius); }

    // Reset causes the ball to re-init itself
    void reset() { *this = Ball(); }
    
    // Ask the ball to bounce in the new direction
    void bounce(DIRECTION dir);
    
    // Ask the ball to move normally on a tick
    void move();
    
    // Ask the ball to move in the direction dir
    void move(DIRECTION dir);
  };
}

// Allows cout printing of the ball and allows us to stringify
//  the ball so we can use it as a key in the image manager.
std::ostream& operator<<(std::ostream& os, const rebound::Ball& ball);

#endif //__BALL_HPP__
