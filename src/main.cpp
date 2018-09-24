#include "main.h"

int main(int argc, char* argv[]) {
  /*GETTING ARGUMENTS*/
  std::cout << "HilMa starting!" << std::endl;
  std::vector<unsigned long> v;
  std::string firstArg(argv[1]);
  std::string name;
  if (firstArg == "-h" || firstArg == "--help" || firstArg == "help") {
    printHelp();
    return 1;
  } else {
    v = readstdin(); // inits a vector v from READING FROM STDIN
    name = std::string(argv[1]); // inits a string name from args
    std::cout << "Vector total size:\t" << v.size() << std::endl;
  }
  // READING VECTOR!
  int r, c;
  computesize(v.size(), &r, &c);
  int pix, piy;
  if (r > c) {
    pix = (16*r)+40+220; //+40 spacers -> + 220 color bar
    piy = (16*c)+40;
  } else {
    piy = (16*r)+40;
    pix = (16*c)+40+220;
  }
  std::cout << "Final plot dimension:\t\t" << pix << "x" << piy << "px" << std::endl;
  Turtle t(v, pix, piy, name);
  turtle_move(t, r, c);
  return 1;
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
  }
  std::cout << std::endl;
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
