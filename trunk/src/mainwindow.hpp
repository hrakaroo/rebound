#ifndef __MAIN_WINDOW_HPP__
#define __MAIN_WINDOW_HPP__

// Needed for painting member
#include "painter.hpp"

// Forward declare
struct SDL_Surface;

namespace rebound {

  // Forward class declaration
  class PlayArea;
  class ScoreArea;
  class Game;
  class ImageMgr;
  class SoundMgr;
  
  class MainWindow
  {
    Painter         _painter;    // The main window screen to paint on 
    const ImageMgr& _imageMgr;   // That which manages our images
    const SoundMgr& _soundMgr;   // That which manages our images
    PlayArea*       _playArea;   // Top play area
    ScoreArea*      _scoreArea;  // Bottom score area
    Game*           _game;       // Set of levels
    short           _countDown;  // How long to wait before starting ball
    bool            _inPause;    // Did they hit pause?
    bool            _inInfo;     // Are they looking at the info screen
    unsigned int    _level;      // Current level we are playing
    
  public:
    MainWindow(Painter& painter,
	       const ImageMgr& imageMgr, 
	       const SoundMgr& soundMgr);

    ~MainWindow();

    // Received from main, this is what advances our game
    void tick();
    
    // Received from main, lets us know of user movements
    void keyDown(const SDL_keysym& keysym);
    void keyUp(const SDL_keysym& keysym);

    void start();
    void end();
    void pause();
    void menu();

  protected:
    void ballLoss();
    void levelCleared();
    void loadLevel();
    void gameOver();
    
  };

}

#endif //__MAIN_WINDOW_HPP__
