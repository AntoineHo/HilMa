#include "Classes.h"

// using namespace std; //

Turtle::Turtle(std::vector<unsigned long> pvector, int pwidth, int pheight, std::string pname) {
  // VARIABLES FROM ARGS
  vector = pvector;
  width = pwidth;
  height = pheight;
  name = pname;
  ren(name);
  // DERIVED VARIABLES
  maxsteps = vector.size();
  vmin = 0;
  vmax = 0;
  curstep = 0;
  breakSignal = false;
  //std::cout << "vector size: " << maxsteps << '\n';

  for (int i(0); i < maxsteps; i++) {
    if (vector[i] < vmin) {
      vmin = vector[i];
    }
    if (vector[i] > vmax) {
      vmax = vector[i];
    }
  }
  //std::cout << vmin << " & " << vmax << std::endl;

  surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
  cr = cairo_create (surface);
  cairo_move_to(cr, 0, 0);
  cairo_set_line_width (cr, 0.02);
  cairo_rectangle (cr, 0, 0, width, height); // WHITE BACKGROUND
  cairo_set_source_rgb (cr, 1, 1, 1);
  cairo_fill (cr);
  cairo_rectangle (cr, 16, 20, width-260, height-35); // GREY BACK
  cairo_set_source_rgb (cr, 0.82, 0.82, 0.82);
  cairo_fill (cr);
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
  std::cout << "Drawing colorbar..." << std::endl;
  // OUTLINE OF COLORBAR
  cairo_set_line_width (cr, 10);
  cairo_rectangle (cr, width-205, 15, 110, height-40); // x-95 y-40
  cairo_set_source_rgb (cr, 0, 0, 0);
  cairo_fill (cr);
  // COLORBAR
  cairo_pattern_t *pat;
  pat = cairo_pattern_create_linear(width-150, 20, width-150, height-45);
  cairo_pattern_add_color_stop_rgb(pat, 0.0, 0.0, 1.0, 0.0);
  cairo_pattern_add_color_stop_rgb(pat, 0.5, 0.0, 0.0, 1.0);
  cairo_pattern_add_color_stop_rgb(pat, 1.0, 1.0, 0.0, 0.0);
  cairo_rectangle(cr, width-200, 20, 100, height-50); // x-100 y-45
  cairo_set_source(cr, pat);
  cairo_fill(cr);
  cairo_pattern_destroy(pat);
  // TEXT OF COLORBAR
  cairo_set_source_rgb (cr, 0, 0, 0);
  cairo_select_font_face(cr, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size(cr, 14);
  cairo_move_to(cr, width-90, 30);
  cairo_show_text(cr, toString(vmin));
  cairo_move_to(cr, width-90, 0.5*height);
  cairo_show_text(cr, toString(vmin + 0.5*(vmax-vmin)));
  cairo_move_to(cr, width-90, height-35);
  cairo_show_text(cr, toString(vmax));
  // EFFECTIVE CAIRO DESTROY
  cairo_surface_write_to_png(surface, std::string(name + ".out.png").c_str());
  cairo_destroy(cr);
  cairo_surface_destroy(surface);

  // CAIRO COLORBAR SECOND FILE
  cairo_surface_t *surface;
  cairo_t *cr;
  surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 300, 1000);
  cr = cairo_create (surface);

  cairo_rectangle (cr, 0, 0, 500, 1000); // WHITE BACKGROUND
  cairo_set_source_rgb (cr, 1, 1, 1);
  cairo_fill (cr);

  cairo_set_line_width (cr, 10);
  cairo_rectangle (cr, 0, 0, 200, 1000); // BLACK BOX
  cairo_set_source_rgb (cr, 0, 0, 0);
  cairo_fill (cr);

  cairo_pattern_t *pat2;
  pat2 = cairo_pattern_create_linear(0, 0, 0, 990); //990 because we lose 10px of blackbox
  cairo_pattern_add_color_stop_rgb(pat2, 0.0, 0.0, 1.0, 0.0);
  cairo_pattern_add_color_stop_rgb(pat2, 0.5, 0.0, 0.0, 1.0);
  cairo_pattern_add_color_stop_rgb(pat2, 1.0, 1.0, 0.0, 0.0);
  cairo_rectangle(cr, 5, 5, 190, 990); // COLORBAR
  cairo_set_source(cr, pat2);
  cairo_fill(cr);
  cairo_pattern_destroy(pat2);

  // TEXT OF COLORBAR
  cairo_set_source_rgb (cr, 0, 0, 0);
  cairo_select_font_face(cr, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size(cr, 24);
  cairo_move_to(cr, 205, 20);
  cairo_show_text(cr, toString(vmin));
  cairo_move_to(cr, 205, 500);
  cairo_show_text(cr, toString(vmin + 0.5*(vmax-vmin)));
  cairo_move_to(cr, 205, 980);
  cairo_show_text(cr, toString(vmax));

  cairo_surface_write_to_png(surface, std::string(name + ".colorbar.png").c_str());
  cairo_destroy(cr);
  cairo_surface_destroy(surface);
  std::cout << "Done!" << std::endl;
}

void Turtle::draw(int x0, int y0, int x1, int y1) {
  //std::cout << "Drawing!" << std::endl;
  std::vector<int> colors;
  for (int i(0); i < 3; i++) {
    colors.push_back(0);
  }
  int cValue(vector[curstep]);
  rgb(vmax, vmin, cValue, colors);
  float r, g, b;
  r =  (float(colors[0])/255);
  g =  (float(colors[1])/255);
  b =  (float(colors[2])/255);
  //std::cout << r << " " << g << " " << b << std::endl;
  cairo_set_source_rgb(cr, r, g, b);  // Solid color
  cairo_set_line_width(cr, 2);

  int nx0(0);
  int ny0(0);
  int nx1(0);
  int ny1(0);
  convert_coordinates(x0, y0, nx0, ny0);
  convert_coordinates(x1, y1, nx1, ny1);
  //std::cout << nx0 << "," << ny0 << " to " << nx1 << "," << ny1 << std::endl;
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
