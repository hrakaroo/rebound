// Our include
#include "ball.hpp"

// Put us in the rebound namespace
namespace rebound {
  
  // When we are asked to bounce in a direction dir
  void 
  Ball::bounce(DIRECTION dir)
  {
    switch (dir) {
    case UP:
      // Bounce it
      _direction = UP;
      // Move it up a bit
      _y -= BALL_SKIP;
      break;
    case DOWN:
      // Bounce it
      _direction = DOWN;
      _y += BALL_SKIP;
      break;
    case LEFT:
      _x -= BALL_BOUNCE * BALL_SKIP;
      break;
    case RIGHT:
      _x += BALL_BOUNCE * BALL_SKIP;
      break;
    }
  }
  
  // A normal move for the ball 
  void 
  Ball::move()
  {
    move(_direction);
  }
  
  // A move for the ball in a given direction
  void 
  Ball::move(DIRECTION dir)
  {
    switch (dir) {
    case UP:
      _y -= BALL_SKIP;
      break;
    case DOWN:
      _y += BALL_SKIP;
      break;
    case LEFT:
      _x -= BALL_SKIP;
      break;
    case RIGHT:
      _x += BALL_SKIP;
      break;
    }
  }
}

// Ostream stringify for a ball
std::ostream& operator<<(std::ostream& os, const rebound::Ball& ball)
{
  os << "ball(" << rebound::colorstr[ball.color()] << ")";
  return os;
}
