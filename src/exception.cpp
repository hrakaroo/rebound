#include "exception.hpp"

std::ostream& operator<<(std::ostream& os, const rebound::Exception& e)
{
  os << "Exception(" << e.errstr() << ")";
  return os;
}
