// Our header
#include "scorearea.hpp"

// Rebound Includes
#include "imagemgr.hpp"
#include "ball.hpp"
#include "rebound.hpp"
#include "exception.hpp"
#include "soundmgr.hpp"

// SDL Includes
#include <SDL.h>

// Std Includes
#include <iostream>
#include <string>
#include <sstream>

// Ease of use
using std::string;
using std::stringstream;
using std::cout;
using std::endl;

namespace rebound {
  
  // Constructor
  ScoreArea::ScoreArea(Painter& painter,
		       const ImageMgr& imageMgr, 
		       const SoundMgr& soundMgr)
    : _painter(painter),
      _imageMgr(imageMgr),
      _soundMgr(soundMgr),
      _ballCount(3),
      _background(_imageMgr.fetch("score")),
      _font(0),
      _score(0),
      _extraBall(0),
      _timeRemaining(100),
      _timerTick(0)
  {
    // Draw the background
    _font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    if(!_font) {
      stringstream sstr;
      sstr << "TTF_OpenFont (" << FONT_PATH << "): " << TTF_GetError();
      throw Exception(sstr.str());
    }

    if (!_background) cout << "What!!!" << endl;

    _painter.drawImage(_background);

    drawBalls();

    // Draw the first score
    drawScore();

    drawTimeRemaining();
  }


  // Destructor
  ScoreArea::~ScoreArea()
  {
    if (_font) TTF_CloseFont(_font);
    _font = 0;
  }

  
  void 
  ScoreArea::reset()
  {
    _score         = 0;
    _ballCount     = 3;
    _extraBall     = 0;
    _timeRemaining = 100;
    _timerTick     = 0;
    _painter.drawImage(_background);
    drawScore();
    drawBalls();
    drawTimeRemaining();
  }

  
  void 
  ScoreArea::drawBalls()
  {
    // Use the generic blue ball
    Ball ball;

    // Fetch the image
    SDL_Surface* ballImage = _imageMgr.fetch(ball);
    
    // We are going to draw up to 4 balls ... 
    for (unsigned short i=0; i<4; ++i) {
      // Get the xloc 
      int x = SCORE_BALLX + i*(2*ball.radius() + 10);
      Rect rect(x, SCORE_BALLY - ball.radius(), 2*ball.radius(), 2*ball.radius());
      
      // Should we draw the ball, or the background
      if (i < _ballCount)
	_painter.drawImage(ballImage, rect);
      else
	_painter.drawImage(_background, rect, rect);
    }
  }

  
  void 
  ScoreArea::tick()
  {
    // Decrement timer
    if (_timeRemaining && ++_timerTick >= 100) {
      --_timeRemaining;
      _timerTick = 0;
      drawTimeRemaining();
    }
  }
  

  void 
  ScoreArea::ballLoss()
  {
    // Remove one of the balls and redraw the extra balls
    -- _ballCount;
    drawBalls();
  }


  // Draw the given author name on the the screen centered at location
  //  AUTHORX, AUTHORY
  void 
  ScoreArea::author(const string& author_)
  {
    // Wipe out the old author if it is not empty
    if (! _authorRect.empty()) {
      _painter.drawImage(_background, _authorRect, _authorRect);
    }

    SDL_Color color={0,0,0};
    SDL_Surface* text = TTF_RenderText_Blended(_font, author_.c_str(), color);
    if (!text)
      throw Exception("Error in writing text");

    // Create the target rect
    _authorRect = Rect(AUTHORX - (text->w/2), AUTHORY - (text->h/2), 
		   text->w, text->h);

    // Paint it
    _painter.drawImage(text, _authorRect);

    // Free the memory
    SDL_FreeSurface(text);
  }


  // Draw the given title on the the screen centered at location
  //  TITLEX, TITLEY
  void
  ScoreArea::title(const string& title_)
  {
    // Wipe out the old title if it is not empty
    if (! _titleRect.empty()) 
      _painter.drawImage(_background, _titleRect, _titleRect);

    SDL_Color color={0,0,0};
    SDL_Surface* text = TTF_RenderText_Blended(_font, title_.c_str(), color);
    if (!text) 
      throw Exception("Error in writing text");

    // Create the target rect
    _titleRect = Rect(TITLEX - (text->w/2), TITLEY - (text->h/2), 
		  text->w, text->h);

    // Paint it
    _painter.drawImage(text, _titleRect);
    
    // Free the memory
    SDL_FreeSurface(text);
  }


  void
  ScoreArea::brickHit(FUNCTION function)
  {
    if (function == BRICK) {
      _score += 10;
    } else if (function == GOLD) {
      _score += 25;
    } else {
      // Nothing to do
      return;
    }

    // Draw the new score
    drawScore();

    // Check if they have hit an extra ball
    if (_score / SCORE_EXTRABALL > _extraBall) {
      // Give them the extra ball
      if (_ballCount < 4) {
	// Play the extra ball sound
	_soundMgr.play("extraball");
	// Give them the extra ball
	++_ballCount;
	// Redraw all the balls ... could be better
	drawBalls();
      }
      // Bump up _extraball so we don't keep doing it
      ++_extraBall;
    }
  }

  
  void
  ScoreArea::drawTimeRemaining()
  {
    // Remove the old timer
    if (!_timerRect.empty()) {
      _painter.drawImage(_background, _timerRect, _timerRect);
    }

    // Stringify the timer
    stringstream sstr;
    sstr << _timeRemaining;

    // Draw the new timer as an image
    SDL_Color color={0,0,0};
    SDL_Surface* text = TTF_RenderText_Blended(_font, sstr.str().c_str(), color);
    if (!text) 
      throw Exception("Error in writing text");

    // Draw the new timer image on the screen
    _timerRect = Rect(TIMERX - (text->w), TIMERY - (text->h/2),
		     text->w, text->h);
    
    // Paint it
    _painter.drawImage(text, _timerRect);
    
    // Free the memory
    SDL_FreeSurface(text);
  }


  void 
  ScoreArea::drawScore()
  {
    // Remove the old score
    if (!_scoreRect.empty()) {
      _painter.drawImage(_background, _scoreRect, _scoreRect);
    }

    // Stringify the score
    stringstream sstr;
    sstr << _score;

    // Draw the new score
    SDL_Color color={0,0,0};
    SDL_Surface* text = TTF_RenderText_Blended(_font, sstr.str().c_str(), color);
    if (!text) 
      throw Exception("Error in writing text");

    // Draw the new score
    _scoreRect = Rect(SCOREX - (text->w), SCOREY - (text->h/2),
		      text->w, text->h);
    
    // Paint it
    _painter.drawImage(text, _scoreRect);
    
    // Free the memory
    SDL_FreeSurface(text);
  }


  void
  ScoreArea::gotKey(COLOR color)
  {
    // Remove the last key regardless
    if (!_keyRect.empty()) {
      // Remove the last key
      _painter.drawImage(_background, _keyRect, _keyRect);
      // Remove the rect
      _keyRect = Rect();
    }

    // If no color (LTBLUE), just return
    if (!color)
      return;
    
    // This will hold the new key image
    SDL_Surface* key = 0;
    
    switch (color) {
    case LTBLUE: // Shoudl never get here
      break;
    case YELLOW:
      key = _imageMgr.fetch("score_key_yellow");
      break;
    case GREEN:
      key = _imageMgr.fetch("score_key_green");
      break;
    case COBALT:
      key = _imageMgr.fetch("score_key_cobalt");
      break;
    case RED:
      key = _imageMgr.fetch("score_key_red");
      break;
    case BROWN:
      key = _imageMgr.fetch("score_key_brown");
      break;
    case ORANGE:
      key = _imageMgr.fetch("score_key_orange");
      break;
    }

    if (!key)
      throw Exception("ScoreArea: Missing key image");

    // Build a new key rect
    _keyRect = Rect(SCORE_KEYX - (key->w/2), SCORE_KEYY - (key->h/2),
		    key->w, key->h);

    _painter.drawImage(key, _keyRect);
  }

  void
  ScoreArea::levelCleared()
  {
    // Give them a point for every time they have left

    // Eventually I would like to give them a visual way to watch
    //  the timer go down and the score go up, but that requires
    //  getting the main loop involved again.  Not impossible, just
    //  more involved so for now I am just going to add the points
    //  directly
    _score += _timeRemaining;
    drawScore();
    _timeRemaining = 100;
    drawTimeRemaining();

    /*
    for (; _timeRemaining > 0; --_timeRemaining) {
      ++_score ;
      drawTimeRemaining();
      drawScore();
    }
    cout << "Done ScoreArea::levelCleared()" << endl;
    */
  }
}
