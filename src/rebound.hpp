// This file contains all of the global consts as well as prototypes
//  for all global functions.

#ifndef __REBOUND_HPP__
#define __REBOUND_HPP__


#include <string>
#include <vector>

struct SDL_Surface;

namespace rebound {

  // Eventually I want to move these into a config file so that a 
  //  game could be created which was of size 640 x 480
  //  or so that someone could build a different score area and move
  //  things around.  No other hard codes should exist in the application.

  const unsigned short ballRadius = 12;
  const unsigned short BALL_SKIP = 2;
  const unsigned short BALL_BOUNCE = 4;

  // The attributes of the screen
  const int SCREEN_WIDTH      = 768;
  const int PLAYAREA_HEIGHT   = 480;
  const int SCOREAREA_HEIGHT  = 120;

  const int SCREEN_HEIGHT     = PLAYAREA_HEIGHT + SCOREAREA_HEIGHT;
  const int SCREEN_BPP        = 16;


  // Constants for drawing in the score area
  const int unsigned SCORE_BALLX = 600;
  const int unsigned SCORE_BALLY = 35;

  const int unsigned SCORE_KEYX = 103;
  const int unsigned SCORE_KEYY = 35;

  // Every SCORE_EXTRABALL give them an extra ball if they have space
  const int unsigned SCORE_EXTRABALL = 2000;

  const int unsigned TITLEX = SCREEN_WIDTH/2;
  const int unsigned TITLEY = 35;
  
  const int unsigned AUTHORX = SCREEN_WIDTH/2;
  const int unsigned AUTHORY = 89;

  const int unsigned SCOREX = 675;
  const int unsigned SCOREY = 89;

  const int unsigned TIMERX = 130;
  const int unsigned TIMERY = 89;

  // Max number of rects to start with in the sdlrect list
  const unsigned int MAXRECTS = 10;

  // How long to sleep between ticks (overall speed of game)
  const unsigned int TICK_INTERVAL = 12;

  // Font path and size we use for the entire game
  extern const char* FONT_PATH;
  const int   FONT_SIZE = 24;

  extern const char * IMAGE_DIR;
  extern const char * PICTURE_DIR;
  extern const char * SOUND_DIR;

  enum COLOR     {LTBLUE, ORANGE, RED, BROWN, YELLOW, GREEN, COBALT};
  enum FUNCTION  {EMPTY,   // No brick
		  WALL,    // Wall, not breakable
		  DEATH,   // Death, will kill the ball if it touches
		  KEY,     // Key, needed to open locks
		  LOCK,    // Lock, breakable with a key of same color
		  PAINT,   // Paints ball for keys and bricks
		  BRICK,   // Breakable if ball is same color
		  SWITCH,  // Switches left and right buttons
		  GOLD};
  enum DIRECTION {UP, DOWN, LEFT, RIGHT};
  
  extern const char * colorstr;
  extern const char * functionstr;


  SDL_Surface *loadImage(const std::string& filename, bool hasAlpha=false);
  
}

#endif //__REBOUND_HPP__
