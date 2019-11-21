// Our header
#include "level.hpp"

// Stl
#include <fstream>
#include <sstream>

// Boost
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_iterator.hpp>
#include <boost/spirit/include/classic_utility.hpp>
#include <boost/spirit/include/classic_actor.hpp>

// Rebound
#include "exception.hpp"

using namespace boost::spirit::classic;
using std::string;
using std::stringstream;

typedef file_iterator<char> iterator_t;

namespace rebound {

  // Forward declare this so we can use it early
  class Game;

  Level::Level() : _row(0)
  {
    // Clear out the board
    for (int x=0; x<12; ++x) {
      for (int y=0; y<12; ++y) {
	_board[x][y] = Brick();
      }
    }
  }
  
  
  void
  Level::board(const string& str)
  {
    if (_row >= 12) {
      return;
    }

    int column = 0;
    for (string::size_type i = 0;
	 i < str.length() && column < 12; i += 2, ++column) {
      if (str[i] == '_') {
	// Empty spot
	_board[column][_row] = Brick();
	continue;
      } 

      if (str[i] == 'D') {
	// Death brick
	_board[column][_row] = Brick(DEATH);
	continue;
      } 

      if (str[i] == 'W') {
	// Wall brick
	_board[column][_row] = Brick(WALL);
	continue;
      } 
      
      if (str[i] == 'G') {
	// Money brick
	_board[column][_row] = Brick(GOLD);
	continue;
      } 
      
      if (str[i] == 'S') {
	// Switch brick
	_board[column][_row] = Brick(SWITCH);
	continue;
      } 
      
      FUNCTION function = BRICK;
      COLOR color = LTBLUE;
      if (str[i] == 'B') {
	function = BRICK;
      } else if (str[i] == 'P') {
	function = PAINT;
      } else if (str[i] == 'L') {
	function = LOCK;
      } else if (str[i] == 'K') {
	function = KEY;
      } else {
	throw Exception(std::string("Don't understand function ") + str[i]);
      }
      
      if (str[i+1] == 'L') {
	// Basic Brick
	color = LTBLUE;
      } else if (str[i+1] == 'C') {
	// Cobalt Brick
	color = COBALT;
      } else if (str[i+1] == 'G') {
	// Green Brick
	color = GREEN;
      } else if (str[i+1] == 'R') {
	// Red Brick
	color = RED;
      } else if (str[i+1] == 'Y') {
	// Yellow Brick
	color = YELLOW;
      } else if (str[i+1] == 'B') {
	// Brown Brick
	color = BROWN;
      } else if (str[i+1] == 'O') {
	// Orange Brick
	color = ORANGE;
      } else {
	throw Exception(std::string("Don't understand color ") + str[i+1]);
      }

      _board[column][_row] = Brick(function, color);
    }
    
    ++_row;
  }
  
  
  // Private Functors
  class SetTitle
  {
  private:
    Game& _game;
  public:
    SetTitle(Game& game) : _game(game) {}
    void operator()(const string& str) const { _game.title(str); }
  };
  
  class NewLevel
  {
  private:
    Game& _game;
  public:
    NewLevel(Game& game) : _game(game) {}
    void operator()(const iterator_t&, const iterator_t&) const 
    { _game.newLevel(); }
  };
  
  class SetLTitle
  {
  private:
    Game& _game;
  public:
    SetLTitle(Game& game) : _game(game) {}
    void operator()(const string& str) const 
    { _game.ltitle(str); }
  };
  
  class SetLAuthor
  {
  private:
    Game& _game;
  public:
    SetLAuthor(Game& game) : _game(game) {}
    void operator()(const string& str) const 
    { _game.lauthor(str); }
  };
  
  class SetLImage
  {
  private:
    Game& _game;
  public:
    SetLImage(Game& game) : _game(game) {}
    void operator()(const string& str) const 
    { _game.limage(str); }
  };
  
  class SetLBoard
  {
  private:
    Game& _game;
  public:
    SetLBoard(Game& game) : _game(game) {}
    void operator()(const string& str) const 
    { _game.lboard(str); }
  };
  
  
  // Text parser for getting words or quoted strings
  struct text_parser
  {
    typedef std::string result_t;
    template <typename ScannerT>
    std::ptrdiff_t
    operator()(ScannerT const& scan, result_t& result) const
    {
      if (scan.at_end()) return -1;
      
      bool in_string = false;
      bool in_quote = false;
      
      std::ptrdiff_t len = 0;
      
      // Get the first char
      char ch = *scan;
      if (ch == '"') {
	in_quote = true;
	++len;
	++scan;
      } else if ((ch >= 'A' && ch <= 'Z') ||
		 (ch >= 'a' && ch <= 'z') ||
		 (ch >= '0' && ch <= '9') ||
		 (ch == '_' || ch == '.' || 
		  ch == '/')) {
	in_string= true;
      } else {
	return -1;
      }
      
      result.clear();
      
      while (!scan.at_end()) {
	ch = *scan;
	if (in_string && ((ch >= 'A' && ch <= 'Z') ||
			  (ch >= 'a' && ch <= 'z') ||
			  (ch >= '0' && ch <= '9') ||
			  (ch == '_' || ch == '.' ||
			   ch == '/'))) {
	  result += *scan;
	} else if (in_quote) {
	  if (ch == '"') {
	    ++scan; // Move past the quote
	    ++len;
	    break;
	  } else {
	    result += *scan;
	  }
	} else {
	  break;
	}
	++len;
	++scan;
      }
      
      return len;
    }
  };
  
  functor_parser<text_parser> text_parser_p;
  
  
  class LevelParser : public grammar<LevelParser>
  {
  private:
    Game& _game;
    
  public:
    LevelParser(Game& game) : _game(game) {}
    
    template<typename ScannerT>
    struct definition
    {
      definition(LevelParser const& self)
      {
	lines     = line >> *line;
	line      = title | levels;
	title     = str_p("title") 
	  >> '=' 
	  >> lexeme_d[text_parser_p[ SetTitle(self._game) ]];
	levels    = level >> *level;
	level     = str_p("level") [NewLevel(self._game)]
	  >> '=' >> '{' >> leveldef >> '}';
	leveldef  = levelline >> *levelline;
	levelline = ltitle | lauthor | limage | lboard;
	ltitle    = str_p("title") 
	  >> '=' 
	  >> lexeme_d[text_parser_p [ SetLTitle(self._game)]];
	lauthor   = str_p("author") 
	  >> '=' 
	  >> lexeme_d[text_parser_p [ SetLAuthor(self._game)]];
	limage    = str_p("image")
	  >> '=' 
	  >> lexeme_d[text_parser_p [ SetLImage(self._game)]];
	lboard    = str_p("board") 
	  >> '=' >> '{' >> boarddef >> '}';
	boarddef  = boardline >> *boardline;
	boardline = lexeme_d[text_parser_p [SetLBoard(self._game)]];
      }
      
      rule<ScannerT> lines, line, title, levels;
      rule<ScannerT> level, levelline, leveldef, ltitle, lauthor, limage;
      rule<ScannerT> lboard, boarddef, boardline;
      
      
      rule<ScannerT> const& start() const { return lines; }
    };
  };


  Game::~Game()
  {
    // Delete all levels
    for (Levels::iterator i = _levels.begin();
	 i != _levels.end(); ++i) {
      delete *i;
    }
  }

  /* I don't know if this is the best way to do this.  It seems
     that passing *this is usually something to be wary of 
  */
  void 
  Game::parse(const string& str)
  {
    // Create a level parser for this game
    LevelParser lp(*this);

    iterator_t infile(str);
    if (!infile)
      throw Exception("Unable to open file: " + str);

    iterator_t end = infile.make_end();

    parse_info<iterator_t> info = boost::spirit::classic::parse(infile, end, lp, space_p);
    
    if (!info.full) {
      stringstream sstr;
      sstr << "Parsing failed at: " << info.stop;
      throw Exception(sstr.str());
    }
  }

  
  void 
  Game::ltitle(const string& str)
  {
    if (!_currentLevel)
      throw Exception("Can't set level title with no level");
    else
      _currentLevel->title(str);
  }

  
  void 
  Game::lauthor(const string& str)
  {
    if (!_currentLevel)
      throw Exception("Can't set level author with no level");
    else 
      _currentLevel->author(str);
  }
  

  void
  Game::limage(const string& str)
  {
    if (!_currentLevel) 
      throw Exception("Can't set the level image with no level");
    else
      _currentLevel->image(str);
  }

  
  void
  Game::lboard(const string& str)
  {
    if (!_currentLevel)
      throw Exception("Can't set the level board with no level");
    else
      _currentLevel->board(str);
  }
  
  void 
  Game::newLevel()
  {
    _currentLevel = new Level; 
    _levels.push_back(_currentLevel); 
  }

}
