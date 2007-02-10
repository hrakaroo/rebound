#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <vector>
#include <string>

#include "brick.hpp"

namespace rebound {

  class Level
  {
  private:
    Brick          _board[12][12];
    std::string    _title;
    std::string    _author;
    std::string    _image;
    unsigned short _row;
    
  public:
    Level();
    ~Level() {}

    // Set
    void brick(int x, int y, const Brick& b) { _board[x][y] = b; }
    void title(const std::string& str)  { _title = str; }
    void author(const std::string& str) { _author = str; }
    void image(const std::string& str)  { _image = str; }
    void board(const std::string& str);
    
    // Fetch
    Brick& brick(int x, int y) { return _board[x][y]; }
    const std::string& title()  const { return _title; }
    const std::string& author() const { return _author; }
    const std::string& image()  const { return _image; }
  };
  
  
  typedef std::vector<Level*> Levels;
  
  
  class Game
  {
  private:
    std::string  _title;
    Levels       _levels;
    Level*       _currentLevel;
    
  public:
    Game() : _currentLevel(0) {}
    ~Game();

    // Parse an input file
    void parse(const std::string& str);
    
    // Set and fetch the title of the game (set of levels)
    void title(const std::string& str) { _title = str; }
    std::string title() const { return _title; }
    
    // Start a new level
    void newLevel();
    
    // Set the current level attributes
    void ltitle(const std::string& str);
    void lauthor(const std::string& str);
    void limage(const std::string& str);
    void lboard(const std::string& str);
    
    const Levels& levels() const { return _levels; }
  };
}

#endif //__LEVEL_H__
