/*
 * geometry.cpp
 *
 *  Created on: Apr 30, 2018
 *      Author: cgbri
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../compchem.hpp"

using namespace compchem;

int main(int argc, char **argv) {
	linkDLLs();
	compchem::Molecule *mol;
	compchem::initialize(&mol, argc, argv);
	mol->computeDists();
	mol->computeAngles();
	mol->computePlaneAngles();
	mol->computeTorsionAngles();
	puts("debug1");
	fflush(stdout);
	mol->translateCOM();
	mol->momentsOfInertia();
	mol->rotations();
	puts("debug2");
	fflush(stdout);
	printDists(*mol);
	printAngles(*mol);
	printMoments(*mol);
	puts("debug3");
	fflush(stdout);
	delete mol;
	puts("debug4");
	fflush(stdout);
	unlinkDLLs();
	puts("debug5");
	fflush(stdout);
	return (0);
}

