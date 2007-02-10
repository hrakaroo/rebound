// Our Header
#include "playarea.hpp"

// Rebound Includes
#include "imagemgr.hpp"
#include "soundmgr.hpp"
#include "exception.hpp"

// SDL Includes
#include <SDL.h>

#include <iostream>

using std::cout;
using std::endl;

namespace rebound {
  
  PlayArea::PlayArea(Painter& painter,
		     const ImageMgr& imageMgr, 
		     const SoundMgr& soundMgr)
    : _painter(painter),
      _imageMgr(imageMgr),
      _soundMgr(soundMgr),
      _leftRequest(false),
      _rightRequest(false),
      _background(0),
      _ballImage(0),
      _tickCount(0),
      _switchActive(false),
      _brickCount(0)
  {
    _ballImage = _imageMgr.fetch(Ball());
    
    // Set the brick width/height
    _brickWidth = width() / 12;
    _brickHeight = height() / 12;
  }


  PlayArea::~PlayArea()
  {
    if (_background) {
      SDL_FreeSurface(_background);
      _background = 0;
    }
  }


  void 
  PlayArea::level(const Level& level_)
  {
    try {
      // Copy in the level
      _level = level_;
      
      if (_background) {
	SDL_FreeSurface(_background);
	_background = 0;
      }
      
      // Get the background
      _background = loadImage(PICTURE_DIR + level_.image());
      
      // Draw on the new background
      if (! _background) 
	throw Exception("PlayArea::level : No background image: " + level_.image());
      
      // Now, draw on all the bricks
      _brickCount = 0;
      for (int x=0; x<12; ++x) {
	for (int y=0; y<12; ++y) {
	  // Trigger a draw for that brick
	  updateBrick(x,y);
	  if (_level.brick(x, y).function() == BRICK ||
	      _level.brick(x, y).function() == GOLD)
	    ++_brickCount;
	}
      }
      
      resetBall();
    } catch (Exception& e) {
      // Tag on some additional info
      throw Exception("PlayArea::level : " + e.errstr());
    }
  }


  void 
  PlayArea::resetBall()
  {
    // Remove the old ball (this could be optimized a lot better)
    short bx = _ball.x() / _brickWidth;
    short by = _ball.y() / _brickHeight;
    for (short i = -1; i <=1; ++i) 
      for (short j = -1; j <=1; ++j) 
	updateBrick(bx+i, by+j);

    // Reset the balls color and remove all keys
    _ball.reset();

    // Put the ball in the last column, and third brick from bottom
    _ball.position(width()  - _brickWidth/2, 
		   height() - (_brickHeight*2 + _brickHeight/2));

    // Cache the new ball image
    _ballImage = _imageMgr.fetch(_ball.color());

    // Turn hand switch off
    _switchActive = false;

    // Signal that we have no key
    gotKey(LTBLUE);
  }
  

  void 
  PlayArea::tick()
  {
    // Remove the old ball (this could be optimized a lot better)
    short bx = _ball.x() / _brickWidth;
    short by = _ball.y() / _brickHeight;
    for (short i = -1; i <=1; ++i) 
      for (short j = -1; j <=1; ++j) 
	updateBrick(bx+i, by+j);
    
    moveBall();
    
    // Draw the ball in the new location
    if (_ballImage)
      _painter.drawImage(_ballImage, _ball.rect());
  }
  
  
  
  // Moves the ball in the direction it was heading
  void 
  PlayArea::moveBall()
  {
    // Ask the ball to move in the direction it was going (UP or DOWN)
    _ball.move();
    
    // Now move the ball RIGHT or LEFT, depending on the pending request
    //  Both keys cancel each other out
    if (_rightRequest) 
      if (_switchActive)
	_ball.move(LEFT);
      else
	_ball.move(RIGHT);

    if (_leftRequest)
      if (_switchActive)
	_ball.move(RIGHT);
      else
	_ball.move(LEFT);
    
    // Check for a collision with the walls
    if (_ball.direction() == UP) {
      
      // Check if the ball is too high
      if (_ball.y() - _ball.radius() < 0)
	_ball.bounce(DOWN);
      
    } else if (_ball.direction() == DOWN) {
      
      // Check if the ball is too high
      if (_ball.y() + _ball.radius() > height()) 
	_ball.bounce(UP);
    }
    
    // Check if the ball is out of bounds to the left
    if (_ball.x() - _ball.radius() < 0) 
      _ball.bounce(RIGHT);
    
    // Check if the ball is out of bounds to the right
    else if (_ball.x() + _ball.radius() > width()) 
      _ball.bounce(LEFT);
    
    // Now we get to checking for brick hits
    // For collision testing we are first going to check the 
    //  four main bricks, top, bottom, left, right
    
    // Determine which brick the ball is *in*
    short bx = _ball.x() / _brickWidth;
    short by = _ball.y() / _brickHeight;
    
    bool hit = false;
    
    // Check for a top brick hit
    if (by > 0 && _level.brick(bx, by-1).function() != EMPTY) {
      // Check for a hit
      if (_ball.y() - _ball.radius() <= by*_brickHeight) {
	_ball.bounce(DOWN);
	if (hitBrick(bx, by-1)) return;
	hit = true;
      }
    } 
    
    // Check for a bottom brick hit
    if (by < 11 && _level.brick(bx, by+1).function() != EMPTY) {
      // Check for a hit
      if (_ball.y() + _ball.radius() >= (by+1)*_brickHeight) {
	_ball.bounce(UP);
	if (hitBrick(bx, by+1)) return;
	hit = true;
      }
    }
    
    // Check for a left brick hit
    if (bx > 0 && _level.brick(bx-1, by).function() != EMPTY) {
      // Check for a hit
      if (_ball.x() - _ball.radius() <= bx*_brickWidth) {
	_ball.bounce(RIGHT);
	if (hitBrick(bx-1, by)) return;
	hit = true;
      }
    } 
    
    // Check for a right brick hit
    if (bx < 11 && _level.brick(bx+1, by).function() != EMPTY) {
      // Check for a hit
      if (_ball.x() + _ball.radius() >= (bx+1)*_brickWidth) {
	_ball.bounce(LEFT);
	if (hitBrick(bx+1, by)) return;
	hit = true;
      }
    } 
    
    if (! hit) {
      // The ball distance must be less than this
      int balldist = _ball.radius()*_ball.radius();
      
      // Now we need to check to corners
      
      // Check the upper left corner first
      if (bx > 0 && by > 0 &&
	  _level.brick(bx-1, by-1).function() != EMPTY &&
	  _ball.x() - _ball.radius() <= bx * _brickWidth &&
	  _ball.y() - _ball.radius() <= by * _brickHeight) {
	// Compute a hit using distance formula
      
	int ydist = _ball.y() - by*_brickHeight;
	ydist *= ydist;
	
	int xdist = _ball.x() - bx*_brickWidth;
	xdist *= xdist;
	
	if ( ydist + xdist <= balldist ) {
	  _ball.bounce(DOWN);
	  _ball.bounce(RIGHT);
	  if (hitBrick(bx-1, by-1)) return;
	} else {
	  // We need to repair this brick as we probably cut off the corner
	  updateBrick(bx-1, by-1);
	}
      }
      
      // Check the upper right corner
      if (bx < 11 && by > 0 &&
	  _level.brick(bx+1, by-1).function() != EMPTY &&
	  _ball.x() + _ball.radius() >= (bx+1) * _brickWidth &&
	  _ball.y() - _ball.radius() <= by * _brickHeight) {
	// Compute a hit using distance formula
	
	int ydist = _ball.y() - by*_brickHeight;
	ydist *= ydist;
	
	int xdist = _ball.x() - (bx+1)*_brickWidth;
	xdist *= xdist;
	
	if ( ydist + xdist <= balldist ) {
	  _ball.bounce(DOWN);
	  _ball.bounce(LEFT);
	  if (hitBrick(bx+1, by-1)) return;
	} else {
	  // We need to repair this brick as we probably cut off the corner
	  updateBrick(bx+1, by-1);
	}
      }
      
      // Check the lower right corner
      if (bx < 11 && by < 11 &&
	  _level.brick(bx+1, by+1).function() != EMPTY &&
	  _ball.x() + _ball.radius() >= (bx+1) * _brickWidth &&
	  _ball.y() + _ball.radius() >= (by+1) * _brickHeight) {
	// Compute a hit using distance formula
	
	int ydist = _ball.y() - (by+1)*_brickHeight;
	ydist *= ydist;
	
	int xdist = _ball.x() - (bx+1)*_brickWidth;
	xdist *= xdist;
	
	if ( ydist + xdist <= balldist ) {
	  _ball.bounce(UP);
	  _ball.bounce(LEFT);
	  if (hitBrick(bx+1, by+1)) return;
	} else {
	  // We need to repair this brick as we probably cut off the corner
	  updateBrick(bx+1, by+1);
	}
      }
      
      // Check the lower left corner first
      if (bx > 0 && by < 11 &&
	  _level.brick(bx-1, by+1).function() != EMPTY &&
	  _ball.x() - _ball.radius() <= bx * _brickWidth &&
	  _ball.y() + _ball.radius() >= (by+1) * _brickHeight) {
	// Compute a hit using distance formula
	
	int ydist = _ball.y() - (by+1)*_brickHeight;
	ydist *= ydist;
	
	int xdist = _ball.x() - bx*_brickWidth;
	xdist *= xdist;
	
	if ( ydist + xdist <= balldist ) {
	  _ball.bounce(UP);
	  _ball.bounce(RIGHT);
	  if (hitBrick(bx-1, by+1)) return;
	} else {
	  // We need to repair this brick as we probably cut off the corner
	  updateBrick(bx-1, by+1);
	}
      }
    }
  }
  

  bool
  PlayArea::hitBrick(int x, int y)
  {
    // Cache the color
    COLOR color = _level.brick(x,y).color();
    // Now, deal with the hit 
    switch (_level.brick(x,y).function()) {
    case EMPTY:
      // We should never get here as a brick hit is checked
      //  prior to calling hitBrick
      break;
    case WALL:
      // Play the sound (as this may lag slighly)
      _soundMgr.play("wall");

      break;
    case DEATH:
      // Play the sound (as this may lag slighly)
      _soundMgr.play("death");

      // Signal a ball loss
      ballLoss();
      break;
    case SWITCH:
      // Play the sound (as this may lag slighly)
      _soundMgr.play("switch");

      // Swap the hand switch
      _switchActive = !_switchActive;
      break;
    case KEY:
      // Check if we have the right color to get the key
      if (color == _ball.color()) {
	// Play the sound
	_soundMgr.play("key");
	if (_ball.key() != color) {
	  // Mark that ball as having the key
	  _ball.key(color);
	  // Signal that we have a key
	  gotKey(color);
	}
      } else {
	// Play the sound of hitting a wall
	_soundMgr.play("wall");
      }
      break;
    case LOCK:
      // Check if we have a key for this lock
      if (color == _ball.key()) {
	// Play the sound (as this may lag slighly)
	_soundMgr.play("lock");

	// Remove the brick
	_level.brick(x,y).function(EMPTY);
	// Update the area the brick occupied
	updateBrick(x,y);
	// Remove the key (no key for ltblue)
	_ball.key(LTBLUE);
      } else {
	// Play the sound of hitting a wall
	_soundMgr.play("wall");
      }
      break;
    case PAINT:
      if (_ball.color() != color) {
	// Play the sound (as this may lag slighly)
	_soundMgr.play("paint");

	// Set the ball color
	_ball.color(color);
	_ballImage = _imageMgr.fetch(Ball(color));
	// Clear the key if the color does not match
	if (_ball.color() != _ball.key()) {
	  _ball.key(LTBLUE);
	  // Signal our key loss
	  gotKey(LTBLUE);
	}
      } else {
	// Play the sound of hitting a wall
	_soundMgr.play("wall");
      }
      break;
    case BRICK:
      if (_level.brick(x,y).color() == _ball.color()) {
	// Play the sound (as this may lag slighly)
	_soundMgr.play("brick");

	// Remove the brick
	_level.brick(x,y).function(EMPTY);
	// Signal the brick hit
	brickHit(BRICK);
	// Update the area the brick occupied
	updateBrick(x,y);
	if (--_brickCount == 0) {
	  // Signal that we have cleared the level
	  levelCleared();
	  // Return true so we don't keep processing the ball hit
	  return true;
	}
      } else {
	// Play the sound of hitting a wall
	_soundMgr.play("wall");
      }
      break;
    case GOLD:
      // Play the sound (as this may lag slighly)
      _soundMgr.play("gold");

      // Remove the brick
      _level.brick(x,y).function(EMPTY);
      // Update the area the brick occupied
      updateBrick(x,y);
      // Signal a rebound hit
      brickHit(GOLD);
      if (--_brickCount == 0) {
	// Signal that we have cleared the level
	levelCleared();
	// Return true so we don't keep processing the ball hit
	return true;
      }
    }
    
    // Return false so we keep processing the ball hit
    return false;
  }
  

  void 
  PlayArea::keyDown(const SDL_keysym& keysym)
  {
    switch (keysym.sym) {
    case SDLK_LEFT:
      _leftRequest = true;
      break;
    case SDLK_RIGHT:
      _rightRequest = true;
      break;
    default:
      break;
    }
  }
  

  void 
  PlayArea::keyUp(const SDL_keysym& keysym)
  {
    switch (keysym.sym) {
    case SDLK_LEFT:
      _leftRequest = false;
      break;
    case SDLK_RIGHT:
      _rightRequest = false;
      break;
    default:
      break;
    }
  }
  

  void 
  PlayArea::updateBrick(int x, int y)
  {
    Rect rect(x*_brickWidth, y*_brickHeight, _brickWidth, _brickHeight);
    
    if (x < 0 || x > 11) return;
    if (y < 0 || y > 11) return;
    
    if (_level.brick(x,y).function() == EMPTY) {
      // Copy the background
      _painter.drawImage(_background, rect, rect);
    } else {
      // Copy the brick image
      _painter.drawImage(_imageMgr.fetch(_level.brick(x,y)), rect);
    }
  }
}
