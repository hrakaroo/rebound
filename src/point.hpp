#ifndef __POINT_HPP__
#define __POINT_HPP__

class Point 
{
  int _x;
  int _y;
  
public:
  Point() : _x(0), _y(0) {}
  Point(int x, int y) : _x(x), _y(y) {}

  inline int x() const { return _x; }
  inline int y() const { return _y; }
  
  inline void x(int xx) { _x = xx; }
  inline void y(int yy) { _y = yy; }
};

#endif //__POINT_HPP__
