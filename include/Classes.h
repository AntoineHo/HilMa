#ifndef CLASSES_H_INCLUDED
#define CLASSES_H_INCLUDED

// STANDARD MODULES
#include <iostream> // Preprocessor indication > read/write from/to console
#include <fstream> // Necessary to read/write from/to files
#include <string>
#include <sstream>
#include <vector>
#include <algorithm> // std::min_element
#include <iterator>  // std::begin, std::end
#include <cairo.h>

// OBJECTS

class Turtle {
public:
  Turtle(std::vector<unsigned long> vector, int width, int height, std::string name);
  void left();
  void right();
  void forward();
  void stop();
  void draw(int x0, int y0, int x1, int y1);
  void rgb(int maximum, int minimum, int value, std::vector<int> &colors);
  void convert_coordinates(int x, int y, int &nx, int &ny);
  bool checkBreak();
  const char* toString(int number);
  void ren(std::string& name);
private:
  std::string name;
  std::vector<unsigned long> vector;
  bool breakSignal;
  int vmin;
  int vmax;
  int maxsteps;
  int curstep;
  int width;
  int height;
  int x = 0;
  int y = 0;
  int o = 1; // Orientation value 0 = UP; 1 DOWN; 2 RIGHT; 3 LEFT
  cairo_surface_t *surface;
  cairo_t *cr;
};


// PROTOTYPES

#endif
