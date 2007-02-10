#ifndef __PLAYAREA_HPP__
#define __PLAYAREA_HPP__

// Rebound Includes
#include "ball.hpp"
#include "level.hpp"
#include "painter.hpp"

// Boost includes
#include <boost/signal.hpp>
#include <boost/bind.hpp>

// Forward declares (faster compile)
struct SDL_keysym;
struct SDL_Surface;

namespace rebound {
  
  // Forward declare
  class ImageMgr;
  class SoundMgr;
  
  class PlayArea
  {
    Painter           _painter;      // What we paint on 
    const ImageMgr&  _imageMgr;      // How we get images 
    const SoundMgr&  _soundMgr;      // How we get and play sounds
    bool             _leftRequest;   // Is there a request to go left
    bool             _rightRequest;  // Is there a request to go right
    SDL_Surface*     _background;    // What is our current background image
    SDL_Surface*     _ballImage;     // What is our current ball image
    unsigned short   _tickCount;     // Tick countdown for pause at start
    bool             _switchActive;  // Are left and right switched
    unsigned short   _brickCount;    // How many bricks need to be cleared
    Ball             _ball;          // Our actual ball
    int              _brickWidth;    // The width of one brick
    int              _brickHeight;   // The height of one brick
    Level            _level;         // Current level data
    
  public:
    // Signals
    boost::signal<void ()>         ballLoss;
    boost::signal<void ()>         levelCleared;
    boost::signal<void (COLOR)>    gotKey; 
    boost::signal<void (FUNCTION)> brickHit;

    // Constructor
    PlayArea(Painter& painter,
	     const ImageMgr& imageMgr, const SoundMgr& soundMgr);
    
    // Destructor
    ~PlayArea();
    
    // Tick tells us to move
    void tick();

    // Reset ball when a level is cleared (or at the start)
    void resetBall();

    // Trigger a key down or up request (left or right)
    void keyDown(const SDL_keysym& keysym);
    void keyUp(const SDL_keysym& keysym);

    // Set the level data
    void level(const Level& level_);
    
    // Fetch the width and height of our painter
    inline int width() const { return _painter.width(); }
    inline int height() const { return _painter.height(); }
    
  protected:
    // Ask the ball to move in it current direction
    void moveBall();

    // Redraw a brick
    void updateBrick(int x, int y);
    
    // Trigger a hit on a brick
    bool hitBrick(int x, int y);
  };
}

#endif //__PLAYAREA_HPP__
