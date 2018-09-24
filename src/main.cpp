#include "main.h"

int main(int argc, char* argv[]) {
  /*GETTING ARGUMENTS*/
  std::cout << "HilMa starting!" << std::endl;
  std::cout << "Parsing arguments..." << std::endl;
  std::vector<unsigned long> nv;
  std::string firstArg(argv[1]);
  std::string name;
  long bin(strtol(argv[2], NULL, 10)); // SET BIN SIZE

  if (firstArg == "-h" || firstArg == "--help" || firstArg == "help") {
    printHelp();
    return 1;
  } else {
    nv = readstdin(); // inits a vector v from READING FROM STDIN
    name = std::string(argv[1]); // inits a string name from args
    std::cout << "Vector total size:\t" << nv.size() << "bp" << std::endl;

  }

  std::vector<unsigned long> v; // FINAL VECTOR
  if (bin > 1) {
    std::cout << "Binning with size:\t" << bin << "bp" << std::endl;
    std::vector<unsigned long>::iterator from = nv.begin();
    std::vector<unsigned long> sub;
    do {
      if(nv.end() - from > bin) {
          sub.assign(from, from + bin);
          from += bin;
          int average(accumulate(sub.begin(), sub.end(), 0)/sub.size());
          v.push_back(average);
      } else {
        sub.assign(from, nv.end());
        int average(accumulate(sub.begin(), sub.end(), 0)/sub.size());
        v.push_back(average);
        nv.clear();
        sub.clear();
      }
    } while (!nv.empty());
  } else {
    v = nv;
  }

  if (v.size() > 400000) {
    std::cout << "Plotting multiple files to avoid overcharge!" << std::endl;
    std::cout << "Each file corresponds to 400Kb (binned average)" << std::endl;
  }

  int vmin(0);
  int vmax(0);
  // Getting vector infos
  for (int i(0); i < v.size(); i++) {
    if (v[i] < vmin) {
      vmin = v[i];
    }
    if (v[i] > vmax) {
      vmax = v[i];
    }
  }

  int chknbr(0);
  // reads chunks :
  std::vector<unsigned long>::iterator from = v.begin();
  std::vector<unsigned long> sub;
  do {
    std::string chkname(makeName(name, chknbr));
    std::cout << "### Plotting " << chkname << std::endl;
    if(v.end() - from > 400000) {
        sub.assign(from, from + 400000);
        from += 400000;
        std::cout << "Chunk size: " << sub.size() << "bp" << std::endl;
        readchunk(sub, vmin, vmax, chkname);
    } else {
        sub.assign(from, v.end());
        //sub.resize(4000000);
        std::cout << "Chunk size: " << sub.size() << "bp" << std::endl;

        readchunk(sub, vmin, vmax, chkname);
        v.clear();
        sub.clear();
    }
    chknbr++;
  } while (!v.empty());

  std::cout << "Outputting colorbar scale..." << std::endl;
  outColorBar(name, vmin, vmax);
  std::cout << "Done" << std::endl;
  return 1;
}

void outColorBar(std::string name, int vmin, int vmax) {
  // CAIRO COLORBAR SECOND FILE
  cairo_surface_t *surface;
  cairo_t *cr;
  //surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 300, 1000);
  surface = cairo_svg_surface_create(std::string(name + ".colorbar.svg").c_str(), 300, 1000);
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
  cairo_show_text(cr, Turtle::toString(vmin));
  cairo_move_to(cr, 205, 500);
  cairo_show_text(cr, Turtle::toString(vmin + 0.5*(vmax-vmin)));
  cairo_move_to(cr, 205, 980);
  cairo_show_text(cr, Turtle::toString(vmax));

  //cairo_surface_write_to_svg(surface, std::string(name + ".colorbar.svg").c_str());
  cairo_destroy(cr);
  cairo_surface_destroy(surface);
}

void readchunk(std::vector<unsigned long> chunk, int vmin, int vmax, std::string chunkname) {
  int r, c;
  int psteps(chunk.size());
  computesize(psteps, &r, &c);
  int pix, piy;
  if (r > c) {
    pix = (16*r)+40; //+40 spacers -> + 220 color bar
    piy = (16*c)+40;
  } else {
    piy = (16*r)+40;
    pix = (16*c)+40;
  }
  std::cout << "Chunk dimension:\t\t" << pix << "x" << piy << "px" << std::endl;
  Turtle t(chunk, pix, piy, vmin, vmax, psteps, chunkname);
  turtle_move(t, r, c);
}


void turtle_move(Turtle &t, int rows, int cols) {
  int cx, cy;
  if (rows < cols) {
    cx = cols;
    cy = rows;
  } else {
    cx = rows;
    cy = cols;
  }

  std::cout << "Rows and columns number:\t" << cx << "x" << cy << std::endl;
  int typewas(-1);
  std::cout << "Plotting starts: '.' = " << cx*16 << "bp plotted" << std::endl;
  for (int y(0); y < cy; y++) {
    if (y % 50 == 0 && y != 0) {
      std::cout << std::endl;
    }
    std::cout << ".";
    if (t.checkBreak()) {
      //std::cout << "Broke at y: " << y << std::endl;
      break;
    }
    for (int x(0); x < cx; x++) {
      if (t.checkBreak()) {
        //std::cout << "Broke at x: " << x << std::endl;
        break;
      }
      //std::cout << typewas << std::endl;
      if (x == 0 && (typewas == -1 || typewas == 4)) {
        hilbert(t, 5);
        typewas = 5;
      } else if (x > 0 && x < cx-1 && (typewas == 0 || typewas == 5)) {
        hilbert(t, 0);
        typewas = 0;
      } else if (x == cx-1 && typewas == 0) {
        hilbert(t, 1);
        typewas = 1;
      } else if (x == 0 && typewas == 1) {
        hilbert(t, 2);
        typewas = 2;
      } else if (x > 0 && x < cx-1 && (typewas == 2 || typewas ==3)) {
        hilbert(t, 3);
        typewas = 3;
      } else if (x == cx-1 && typewas == 3) {
        hilbert(t, 4);
        typewas = 4;
      } else {
        hilbert(t, 5);
        typewas = 5;
      }
    }
    //t.CairoStroke(); // STROKE AT EACH END OF LINE -> reduced svg size
  }
  std::cout << std::endl;
   t.CairoStroke(); // STROKE BEFORE STOPPING -> reduced svg size
  t.stop();
}

int computesize(int vecsize, int *rows, int *cols) {
  if (vecsize < 16) {
    *rows = 1;
    *cols = 1;
    return 1;
  }
  float chunks((vecsize/16)+1.0);
  std::cout << "N(Chunks):\t\t" << chunks << '\n';
  int nX(floor(sqrt(chunks)));
  int nY(nX-1);
  while (nX*nY < chunks) {
    nY++;
  }
  *rows = nX;
  *cols = nY;
  return 1;
}

void hilbert(Turtle &t, int type) {
  int a(0);
  switch (type) {
    case 0:
    t.forward(); t.forward(); t.right(); t.forward(); t.right(); t.forward();
    t.left(); t.forward(); t.forward(); t.left(); t.forward(); t.left();
    t.forward(); t.right(); t.forward(); t.right(); t.forward(); t.left();
    t.forward(); t.left(); t.forward(); t.forward(); t.left(); t.forward();
    t.right(); t.forward(); t.right(); t.forward();
    break;

    case 1:
    t.forward(); t.right(); t.forward(); t.left(); t.forward();  t.left();
    t.forward(); t.right(); t.forward(); t.forward(); t.right(); t.forward();
    t.right(); t.forward(); t.left(); t.forward(); t.left(); t.forward();
    t.right(); t.forward(); t.right(); t.forward(); t.forward(); t.right();
    t.forward(); t.left(); t.forward(); t.left(); t.forward();
    break;

    case 2:
    t.forward(); t.forward(); t.left(); t.forward(); t.left(); t.forward();
    t.right(); t.forward(); t.forward(); t.right(); t.forward(); t.right();
    t.forward(); t.left();  t.forward(); t.left(); t.forward(); t.right();
    t.forward(); t.right(); t.forward(); t.forward(); t.right();  t.forward();
    t.left(); t.forward(); t.left(); t.forward(); t.right(); t.forward();
    break;

    case 3:
    t.forward(); t.right(); t.forward(); t.right(); t.forward(); t.left();
    t.forward(); t.forward(); t.left(); t.forward(); t.left(); t.forward();
    t.right(); t.forward(); t.right(); t.forward(); t.left(); t.forward();
    t.left(); t.forward(); t.forward(); t.left(); t.forward(); t.right();
    t.forward(); t.right(); t.forward();  t.forward();
    break;

    case 4:
    t.forward(); t.right(); t.forward(); t.right(); t.forward(); t.left();
    t.forward(); t.forward(); t.left(); t.forward(); t.left(); t.forward();
    t.right(); t.forward(); t.right(); t.forward(); t.left(); t.forward();
    t.left(); t.forward(); t.forward(); t.left(); t.forward(); t.right();
    t.forward(); t.right(); t.forward(); t.left();
    break;

    case 5:
    t.forward(); t.left(); t.forward(); t.right(); t.forward(); t.right();
    t.forward(); t.left(); t.forward(); t.forward(); t.left(); t.forward();
    t.left(); t.forward(); t.right(); t.forward(); t.right(); t.forward();
    t.left(); t.forward(); t.left(); t.forward(); t.forward(); t.left();
    t.forward(); t.right(); t.forward(); t.right(); t.forward();
    break;
  }
}

std::vector<unsigned long> readstdin() {

  std::vector<unsigned long> v( std::istream_iterator<unsigned long> (std::cin),
                                std::istream_iterator<unsigned long> ()  );
  return v;
}

void printHelp() {
  std::cerr << "HilMa builds an hilbert curve from a vector" << std::endl
  << "Usage: " << std::endl
  << "command | Hilma name" << std::endl
  << "\tname\tpath to the .svg output file." << std::endl
  << "HilMa -h\tprints this message" << std::endl;
}

std::string makeName(std::string name, int nb) {
  std::string toReturn;
  std::stringstream ss;
  ss << name << "." << nb;
  ss >> toReturn;
  return toReturn;
}
