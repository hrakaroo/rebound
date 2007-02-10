#include "mainwindow.hpp"

// Rebound Includes
#include "playarea.hpp"
#include "scorearea.hpp"
#include "imagemgr.hpp"
#include "soundmgr.hpp"
#include "painter.hpp"
#include "level.hpp"

// STL/Boost Includes
#include <iostream>
#include <string>
#include <boost/signal.hpp>
#include <boost/bind.hpp>

// SDL Includes
#include <SDL.h>

// Enable some stl names into our namespace
using std::string;
using std::cout;
using std::cerr;
using std::endl;

namespace rebound {
  
  MainWindow::MainWindow(Painter& painter,
			 const ImageMgr& imageMgr,
			 const SoundMgr& soundMgr)
    : _painter(painter), 
      _imageMgr(imageMgr),
      _soundMgr(soundMgr),
      _playArea(0), 
      _scoreArea(0), 
      _game(0),
      _countDown(50),
      _inPause(false),
      _inInfo(false),
      _level(0)
  {
    // Load the game levels
    _game = new Game;

    // Parse the levels file
    _game->parse("./data/levels.ldf");

    Painter playAreaPainter = painter.area(Rect(0, 0, 
						painter.width(), 
						PLAYAREA_HEIGHT));

    _playArea = new PlayArea(playAreaPainter, _imageMgr, _soundMgr);

    // Connect the playArea ballLoss to our ballLoss
    _playArea->ballLoss.connect(boost::bind(&MainWindow::ballLoss, this));

    // Connect the playArea levelCleared to our levelCleared
    _playArea->levelCleared.connect(boost::bind(&MainWindow::levelCleared, this));

    // Create the painter for our score area
    Painter scoreAreaPainter = painter.area(Rect(0, PLAYAREA_HEIGHT,
						 painter.width(), 
						 SCOREAREA_HEIGHT));
    // Create our score area
    _scoreArea = new ScoreArea(scoreAreaPainter, _imageMgr, _soundMgr);

    // Connect the playArea gotKey to the score area gotKey
    _playArea->gotKey.connect(boost::bind(&ScoreArea::gotKey, _scoreArea, _1));

    // Connect the brickHit to the score area
    _playArea->brickHit.connect(boost::bind(&ScoreArea::brickHit, _scoreArea, _1));

    // Show the info screen
    menu();
  }

  MainWindow::~MainWindow()
  {
    if (_playArea) delete _playArea;
    if (_scoreArea) delete _scoreArea;
  }
  
  void MainWindow::tick()
  {
    if (_inInfo or _inPause) return;

    // Tick our child widgets
    if (_countDown) {
      -- _countDown;
    } else {
      _playArea->tick();
      _scoreArea->tick();
    }
  }
  
  void MainWindow::keyDown(const SDL_keysym& keysym)
  {
    _playArea->keyDown(keysym);
  }
  
  void MainWindow::keyUp(const SDL_keysym& keysym)
  {
    _playArea->keyUp(keysym);
  }


  void 
  MainWindow::ballLoss()
  {
    if (_scoreArea->ballCount()) {
      // Reset the ball in the play area
      _playArea->resetBall();
      // Remove a ball from the score area
      _scoreArea->ballLoss();
      _countDown = 50;
    } else {
      // Game over
      gameOver();
      
    }
  }

  void
  MainWindow::gameOver()
  {
    _inInfo = true;

    // Show the info screen
    SDL_Surface* gameOverScreen = _imageMgr.fetch("gameover");
    Rect r( (_painter.width() - gameOverScreen->w)/2,
	    (_painter.height() - gameOverScreen->h)/2, 
	    gameOverScreen->w, 
	    gameOverScreen->h);

    _painter.drawImage(gameOverScreen, r);
  }


  void
  MainWindow::menu()
  {
    _inInfo = true;

    SDL_Surface* infoScreen = _imageMgr.fetch("info");
    Rect r( (_painter.width() - infoScreen->w)/2,
	    (_painter.height() - infoScreen->h)/2, 
	    infoScreen->w, 
	    infoScreen->h);

    _painter.drawImage(infoScreen, r);
  }


  void
  MainWindow::levelCleared()
  {
    // Ask the score area to count the remaining time, if any
    _scoreArea->levelCleared();

    // It would be nice to pause here for a moment
    
    // Fetch the next level and start it
    const Levels& levels = _game->levels();
    if (++_level >= levels.size()) _level = 0;
    
    loadLevel();
  }
  

  void 
  MainWindow::loadLevel()
  {
    const Levels& levels = _game->levels();

    // Load the next level
    _playArea->level(*(levels[_level]));
    
    // Set the title and author
    _scoreArea->title(levels[_level]->title());
    _scoreArea->author(levels[_level]->author());
    _countDown = 50;
  }

  void
  MainWindow::start()
  {
    _inInfo = false;
    _inPause = false;
    _level = 0;
    _scoreArea->reset();

    // Load the first level of the game
    loadLevel();
  }

  void
  MainWindow::end()
  {
    if (_inInfo) return;
    gameOver();
  }

  void
  MainWindow::pause()
  {
    _inPause = !_inPause;
  }



}
