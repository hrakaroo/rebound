// Include our header
#include "brick.hpp"

// Ostream printing for stringify
std::ostream& operator<<(std::ostream& os, const rebound::Brick& brick)
{
  os << "brick(" << rebound::colorstr[brick.color()] 
     << ", " << rebound::functionstr[brick.function()] << ")";
  return os;
}
