#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

// STANDARD MODULES
#include <iostream> // Preprocessor indication > read/write from/to console
#include <string> // Necessary to use strings
#include <vector> // Necessary to use dynamically allocated tables
#include <fstream> // Necessary to read/write from/to files
#include <cmath> // Necessary to use floor and square root functions
#include <sstream> // Necessary to read & fill from cin
#include <iterator> // Necessary to read & fill from cin
#include <numeric> // Necessary to use accumulate
#include <cmath> // Necessary for pow command
#include <cairo.h>
#include <cairo-svg.h>

// LOCAL MODULES
#include "Classes.h"

// DECLARATION OF DEFINES

// PROTOTYPES OF FUNCTIONS
void turtle_move(Turtle &t, int rows, int cols);
int computesize(int vecsize, int *rows, int *cols);
void hilbert(Turtle &t, int type);
void printHelp();
std::vector<unsigned long> readstdin();
void readchunk(std::vector<unsigned long> chunk, int vmin, int vmax, std::string chunkname);
std::string makeName(std::string name, int nb);
void outColorBar(std::string name, int vmin, int vmax);

#endif // INTRO_H_INCLUDED
