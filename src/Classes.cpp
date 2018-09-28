#include "Classes.h"

// using namespace std; //

Turtle::Turtle(std::vector<unsigned long> pvector, int pwidth, int pheight, int pvmin, int pvmax, int psteps, std::string pname) {
  // VARIABLES FROM ARGS
  vector = pvector;
  width = pwidth;
  height = pheight;
  name = pname;
  ren(name);
  maxsteps = psteps;
  curstep = 0;
  vmin = pvmin;
  vmax = pvmax;
  breakSignal = false;

  //surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
  surface = cairo_svg_surface_create(std::string(name + ".svg").c_str(), width, height);
  cr = cairo_create(surface);
  cairo_move_to(cr, 0, 0);
  cairo_set_line_width(cr, 0.02);
  cairo_rectangle(cr, 0, 0, width, height); // WHITE BACKGROUND
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_fill(cr);
  cairo_rectangle(cr, 15, 15, width-30, height-30); // GREY BACK
  cairo_set_source_rgb(cr, 0.82, 0.82, 0.82);
  cairo_fill(cr);
}

void Turtle::left() {
  //std::cout << "Rotating left!" << std::endl;
  switch (o) {
    case 0 : o = 3; break;
    case 1 : o = 2; break;
    case 2 : o = 0; break;
    case 3 : o = 1; break;
  }
}

void Turtle::right() {
  //std::cout << "Rotating right!" << std::endl;
  switch (o) {
    case 0 : o = 2; break;
    case 1 : o = 3; break;
    case 2 : o = 1; break;
    case 3 : o = 0; break;
  }
}

void Turtle::forward() {
  //std::cout << "Moving forward!" << std::endl;
  int x0(x);
  int y0(y);
  switch (o) {
    case 0: y -= 1; break;
    case 1: y += 1; break;
    case 2: x += 1; break;
    case 3: x -= 1; break;
  }
  //std::cout << "curstep: " << curstep << "\tmaxstep: " << maxsteps << std::endl;
  if (curstep < maxsteps) {
    draw(x0, y0, x, y);
  } else if (curstep == maxsteps) {
    breakSignal = true;
  } else {
    int a(0);
  }
  curstep++;

}

void Turtle::stop() {
  // EFFECTIVE CAIRO DESTROY
  cairo_destroy(cr);
  cairo_surface_destroy(surface);
  std::cout << "Done!" << std::endl;
}

void Turtle::CairoStroke() {
  cairo_stroke(cr);
}

void Turtle::draw(int x0, int y0, int x1, int y1) {
  //std::cout << "Drawing!" << std::endl;
  int cValue(vector[curstep]);
  float r, g, b;
  if (cValue > vmax) { // THEN MUST USE BLACK
    r = 0.0;
    g = 0.0;
    b = 0.0;
  } else if (cValue < vmin) {
    r = 1.0;
    g = 1.0;
    b = 1.0;
  } else {
    std::vector<int> colors;
    for (int i(0); i < 3; i++) {
      colors.push_back(0);
    }
    rgb(vmax, vmin, cValue, colors);
    r =  (float(colors[0])/255);
    g =  (float(colors[1])/255);
    b =  (float(colors[2])/255);
  }
  //std::cout << r << " " << g << " " << b << std::endl;
  cairo_set_source_rgb(cr, r, g, b);  // Solid color
  cairo_set_line_width(cr, 1);

  int nx0(0);
  int ny0(0);
  int nx1(0);
  int ny1(0);
  convert_coordinates(x0, y0, nx0, ny0);
  convert_coordinates(x1, y1, nx1, ny1);
  cairo_move_to(cr, nx0, ny0);
  cairo_line_to(cr, nx1, ny1);
  cairo_stroke(cr);
}

void Turtle::rgb(int maximum, int minimum, int value, std::vector<int> &colors) {
  //std::cout << value << std::endl;
  float ratio((float) 2 * (value - minimum) / (maximum - minimum));
  //std::cout << "ratio: " << ratio << std::endl;
  if (0 > int(255*(1 - ratio))) {
    colors[1] = 0; // BLUE colors[1]
  } else {
    colors[1] = int(255*(1 - ratio));
  }
  if (0 > int(255*(ratio - 1))) {
    colors[0] = 0; // RED colors[0]
  } else {
    colors[0] = int(255*(ratio - 1));
  }
  colors[2] = 255 - colors[0] - colors[1];
}

void Turtle::convert_coordinates(int x, int y, int &nx, int &ny) {
  //std::cout << "A("<<x<<","<<y<<") converted -> B("<<nx<<","<<ny<<")"<< std::endl;
  nx = 4*x+20;
  ny = 4*y+20;
  //std::cout << "A("<<x<<","<<y<<") converted -> B("<<nx<<","<<ny<<")"<< std::endl;
}

bool Turtle::checkBreak() {
  //std::cout << "breakSignal = " << breakSignal << std::endl;
  if (breakSignal) {
    return true;
  } else {
    return false;
  }
}

const char* Turtle::toString(int number) {
  std::string toReturn;
  std::stringstream strstream;
  strstream << number;
  strstream >> toReturn;
  return toReturn.c_str();
}

void Turtle::ren(std::string& name) {
  std::string nname;
  for (int i(0); i < name.size(); i++) {
    switch (name[i]) {
      case '/' : nname += "."; break;
      case '\\' : nname += "."; break;
      case '?' : nname += "."; break;
      case '%' : nname += "."; break;
      case '*' : nname += "."; break;
      case ':' : nname += "."; break;
      case '|' : nname += "."; break;
      case '\"' : nname += "."; break;
      case '\'' : nname += "."; break;
      case '>' : nname += "."; break;
      case '<' : nname += "."; break;
      case ' ' : nname += "."; break;
      case '\n' : nname += "."; break;
      case '\t' : nname += "."; break;
      default : nname += name[i];
    }
  }
  name = nname;
}
