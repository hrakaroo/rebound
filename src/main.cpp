// STL Includes
#include <iostream>
#include <string>

// SDL Includes
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

// Rebound Includes
#include "ball.hpp"
#include "brick.hpp"
#include "mainwindow.hpp"
#include "painter.hpp"
#include "rebound.hpp"
#include "sdlrects.hpp"
#include "exception.hpp"
#include "soundmgr.hpp"
#include "imagemgr.hpp"

using std::cerr;
using std::endl;
using std::cout;
using rebound::Ball;
using rebound::Brick;

void
loadImages(rebound::ImageMgr& imageMgr)
{
  // Load up the ball
  imageMgr.load(Ball(),       "ball_ltblue.png", true);
  imageMgr.load(Ball(rebound::BROWN),  "ball_brown.png",  true);
  imageMgr.load(Ball(rebound::COBALT), "ball_cobalt.png", true);
  imageMgr.load(Ball(rebound::GREEN),  "ball_green.png",  true);
  imageMgr.load(Ball(rebound::ORANGE), "ball_orange.png", true);
  imageMgr.load(Ball(rebound::RED),    "ball_red.png",    true);
  imageMgr.load(Ball(rebound::YELLOW), "ball_yellow.png", true);
  
  // Load up the basic images
  imageMgr.load(Brick(rebound::WALL),   "wall.png"  );
  imageMgr.load(Brick(rebound::DEATH),  "death.png" );
  imageMgr.load(Brick(rebound::SWITCH), "switch.png");
  imageMgr.load(Brick(rebound::GOLD),   "gold.png"  );
  
  // Load up all bricks
  imageMgr.load(Brick(rebound::BRICK),         "brick_ltblue.png");
  imageMgr.load(Brick(rebound::BRICK, rebound::COBALT), "brick_cobalt.png");
  imageMgr.load(Brick(rebound::BRICK, rebound::GREEN),  "brick_green.png" );
  imageMgr.load(Brick(rebound::BRICK, rebound::RED),    "brick_red.png"   );
  imageMgr.load(Brick(rebound::BRICK, rebound::YELLOW), "brick_yellow.png");
  imageMgr.load(Brick(rebound::BRICK, rebound::BROWN),  "brick_brown.png" );
  imageMgr.load(Brick(rebound::BRICK, rebound::ORANGE), "brick_orange.png");
  
  // Load up all paint
  imageMgr.load(Brick(rebound::PAINT, rebound::COBALT), "paint_cobalt.png");
  imageMgr.load(Brick(rebound::PAINT, rebound::GREEN),  "paint_green.png" );
  imageMgr.load(Brick(rebound::PAINT, rebound::RED),    "paint_red.png"   );
  imageMgr.load(Brick(rebound::PAINT, rebound::YELLOW), "paint_yellow.png");
  imageMgr.load(Brick(rebound::PAINT, rebound::BROWN),  "paint_brown.png" );
  imageMgr.load(Brick(rebound::PAINT, rebound::ORANGE), "paint_orange.png");
  
  // Load up all keys
  imageMgr.load(Brick(rebound::KEY, rebound::COBALT), "key_cobalt.png");
  imageMgr.load(Brick(rebound::KEY, rebound::GREEN),  "key_green.png" );
  imageMgr.load(Brick(rebound::KEY, rebound::RED),    "key_red.png"   );
  imageMgr.load(Brick(rebound::KEY, rebound::YELLOW), "key_yellow.png");
  imageMgr.load(Brick(rebound::KEY, rebound::BROWN),  "key_brown.png" );
  imageMgr.load(Brick(rebound::KEY, rebound::ORANGE), "key_orange.png");
  
  // Load up all locks
  imageMgr.load(Brick(rebound::LOCK, rebound::COBALT), "lock_cobalt.png");
  imageMgr.load(Brick(rebound::LOCK, rebound::GREEN),  "lock_green.png" );
  imageMgr.load(Brick(rebound::LOCK, rebound::RED),    "lock_red.png"   );
  imageMgr.load(Brick(rebound::LOCK, rebound::YELLOW), "lock_yellow.png");
  imageMgr.load(Brick(rebound::LOCK, rebound::BROWN),  "lock_brown.png" );
  imageMgr.load(Brick(rebound::LOCK, rebound::ORANGE), "lock_orange.png");
  
  imageMgr.load("score", "score.png");
  imageMgr.load("score_key_green", "score_key_green.png", true);
  imageMgr.load("score_key_yellow", "score_key_yellow.png", true);
  imageMgr.load("score_key_red", "score_key_red.png", true);
  imageMgr.load("score_key_cobalt", "score_key_cobalt.png", true);
  imageMgr.load("score_key_brown", "score_key_brown.png", true);
  imageMgr.load("score_key_orange", "score_key_orange.png", true);
  imageMgr.load("info", "info.png", true);
  imageMgr.load("gameover", "gameover.png", true);
}

void 
loadSounds(rebound::SoundMgr& soundMgr)
{
  soundMgr.load("gold",      "gold.wav");
  soundMgr.load("wall",      "wall.wav");
  soundMgr.load("paint",     "paint.wav");
  soundMgr.load("death",     "death.wav");
  soundMgr.load("lock",      "lock.wav");
  soundMgr.load("key",       "speedup.wav");
  soundMgr.load("switch",    "switch.wav");
  soundMgr.load("brick",     "brick.wav");
  soundMgr.load("extraball", "extraball.wav");
}



int main(int argc, char* argv[]) 
{
  // Init SDL, SDL_ttf, SDL_mixer
  if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO) == -1) {
    cerr << "Could not init SDL: " << SDL_GetError() << endl;
    return 1;
  }
  if (TTF_Init() == -1) {
    cerr << "Could not init TTF" << endl;
    return 1;
  }
  if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, 
		    //		    MIX_DEFAULT_FORMAT, 1, 1024) == -1) {
		    MIX_DEFAULT_FORMAT, 1, 512) == -1) {
    cerr << "Could not open audio: " << Mix_GetError() << endl;
    return 1;
  }


  // Declare it outside the try block so we can delete it after
  SDL_Surface* screen = 0;

  // Wrap it all in a try/catch so we can deal with errors in one place
  try {

    // Create our main screen for drawing
    //  WIDTH and HEIGHT are obvious, but the use of SWSURFACE is 
    //  because we are drawing images with alpha channels.
    screen = SDL_SetVideoMode( rebound::SCREEN_WIDTH, 
			       rebound::SCREEN_HEIGHT, 
			       rebound::SCREEN_BPP, 
			       SDL_SWSURFACE );
    
    if (!screen) {
      throw rebound::Exception("Could not create SDL scrren");
    }
  
    // Set some text for the window and when it is an icon
    SDL_WM_SetCaption("Rebound", "Bounding");
    
    // Hide the mouse, we don't need it
    SDL_ShowCursor(SDL_DISABLE);
    
    // Load the images
    rebound::ImageMgr imageMgr(rebound::IMAGE_DIR);
    loadImages(imageMgr);
    
    // Load the sounds
    rebound::SoundMgr soundMgr(rebound::SOUND_DIR);
    loadSounds(soundMgr);
    
    // This will hold our update rects
    rebound::SDLRects updateRects;
    
    // Wrap a painter around the screen
    rebound::Painter painter(screen, updateRects,
			     rebound::Rect(0, 0, 
					   rebound::SCREEN_WIDTH, 
					   rebound::SCREEN_HEIGHT));

    // Create our mainwindow
    rebound::MainWindow mainWindow(painter, imageMgr, soundMgr);
    
    // Init our timer for our interval
    unsigned int nextTime = SDL_GetTicks() + rebound::TICK_INTERVAL;
    
    // Draw the entire screen in a flip (same as UpdateRect)
    if (SDL_Flip( screen ) == -1) {
      cout << "Could not call initial SDL_Flip" << endl;
      return 1;
    }
    
    // Mark everything as clean
    updateRects.reset();
    
    // Loop until we quit
    bool quit = false;
    while (! quit) {
      
      // Update the screen 
      SDL_UpdateRects(screen, updateRects.numRects(), updateRects.rects());
      updateRects.reset();
      
      // Tick the screens
      mainWindow.tick();
      
      // Loop until there are no pending events
      SDL_Event event;
      while (SDL_PollEvent(&event)) {
	switch (event.type) {
	case SDL_QUIT:
	  quit = true;
	  break;
	case SDL_KEYDOWN:
	  if (event.key.keysym.sym == SDLK_q) {
	    quit = true;
	    break;
	  } else if (event.key.keysym.sym == SDLK_s) {
	    mainWindow.start();
	  } else if (event.key.keysym.sym == SDLK_e) {
	    mainWindow.end();
	  } else if (event.key.keysym.sym == SDLK_p) {
	    mainWindow.pause();
	  } else if (event.key.keysym.sym == SDLK_m) {
	    mainWindow.menu();
	  }
	  // Send the key to the playArea
	  mainWindow.keyDown(event.key.keysym);
	  break;
	case SDL_KEYUP:
	  mainWindow.keyUp(event.key.keysym);
	  break;
	default:
	  break;
	}
      }
      
      // Sleep until the next interval
      unsigned int now = SDL_GetTicks();
      if (now < nextTime)
	SDL_Delay(nextTime - now);
      nextTime = SDL_GetTicks() + rebound::TICK_INTERVAL;
    }
  } catch (rebound::Exception& e) {
    cerr << e << endl;
  }
  
  // Show the mouse, return to a nice state, might not be needed
  SDL_ShowCursor(SDL_ENABLE);
    
  // Free up the memory from our main screen
  if (screen) SDL_FreeSurface(screen);

  // Shutdown all SDL cleanly
  SDL_Quit();
  TTF_Quit();
  Mix_CloseAudio();
  
  // No errors
  return 0;
}
