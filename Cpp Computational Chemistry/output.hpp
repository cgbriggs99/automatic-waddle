/*
 * output.cpp
 *
 *  Created on: Apr 27, 2018
 *      Author: cgbri
 */

#ifndef OUTPUT_CPP_
#define OUTPUT_CPP_

#include <stdio.h>
#include <stdlib.h>
#include "compchem.hpp"

namespace compchem {
using namespace compchem;

extern void printDists(Molecule *molecule);

extern void printAngles(Molecule *molecule);

extern void printRotor(Molecule *molecule);

extern void printMoments(Molecule *molecule);

extern void printEigenvalues(Molecule *molecule);

extern void printFreqs(Molecule *molecule);

extern void printSCF(Molecule *molecule);

extern void printArray(char *title, Array<double> *data);

extern void printMP2(Molecule *molecule);

}

#endif /* OUTPUT_CPP_ */
