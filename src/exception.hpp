#ifndef __EXCEPTION_HPP__
#define __EXCEPTION_HPP__

#include <string>
#include <ostream>

namespace rebound {
 
  class Exception
  {
  private:
    std::string _errstr;

  public:
    // Basic constructor
    Exception(const std::string& str) : _errstr(str) {}

    // Copy constructor
    Exception(const Exception& e) : _errstr(e._errstr) {}
    
    // Fetch the string, duh
    const std::string& errstr() const { return _errstr; }

  private:
    // Not allowed an empty constructor
    Exception() {}
  };
}

std::ostream& operator<<(std::ostream& os, const rebound::Exception& e);

#endif //__EXCEPTION_HPP__
