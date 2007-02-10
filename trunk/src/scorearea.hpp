#ifndef __SCORE_AREA_HPP__
#define __SCORE_AREA_HPP__

/* Manages the lower score area of the game window */

#include "painter.hpp"
#include "rebound.hpp"

// Can't forward declare TTF_Font without it getting messy
#include <SDL_ttf.h>

namespace rebound {
  
  class ImageMgr;
  class SoundMgr;
  
  class ScoreArea
  {
    Painter         _painter;       // How we paint on the screen
    const ImageMgr& _imageMgr;      // That which holds our images
    const SoundMgr& _soundMgr;      // That which holds our sounds
    short unsigned  _ballCount;     // How many balls are left
    SDL_Surface*    _background;    // Background image of score area
    TTF_Font*       _font;          // Font we are using
    int unsigned    _score;         // Accumulated points
    int unsigned    _extraBall;     // How many extra balls have they gotten
    int unsigned    _timeRemaining; // Time to still get bonus points
    int unsigned    _timerTick;     
    Rect            _titleRect;     // Rect title was drawn in   
    Rect            _authorRect;    // Rect author was drawn in
    Rect            _keyRect;  
    Rect            _scoreRect;  
    Rect            _timerRect;
    
  public:
    // Constructor
    ScoreArea(Painter& painter,
	      const ImageMgr& imageMgr, 
	      const SoundMgr& soundMgr);

    ~ScoreArea();

    void tick();      // Let the area know a moment has passed
    void reset();     // Reset the score for a new game
    void drawBalls(); // Draw the number of balls we have
    void ballLoss();  // Register a ball loss
    void drawTimeRemaining();
    void levelCleared();

    // Fetch the number of balls left
    inline short unsigned ballCount() const { return _ballCount; }

    // Set the title, author
    void title(const std::string& title_);
    void author(const std::string& author_);

    void gotKey(COLOR color);

    void brickHit(FUNCTION function);
    
    inline int unsigned score() const { return _score; }

  protected:
    void drawScore();

  private:
    // No empty construction
    // ScoreArea() {}  Not needed as no empty painter contructors

    // No copying
    // ScoreArea( const ScoreArea& ) {}
  };
}

#endif //__SCORE_AREA_HPP__
