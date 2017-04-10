/*! \file main.cpp
 *   This is the main file where is the main function of the program
 */

#include "amrosapp.h"

int main(int argc, char *argv[]) {

  int outputApp;
  
  AmrosApp amrosApp(argc,argv);

  outputApp = amrosApp.run();

  return outputApp;
}

