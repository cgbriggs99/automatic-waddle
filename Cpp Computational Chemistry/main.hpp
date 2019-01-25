/*
 * main.hpp
 *
 *  Created on: Apr 30, 2018
 *      Author: cgbri
 */

#ifndef MAIN_HPP_
#define MAIN_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "compchem.hpp"


namespace compchem {

void initialize_geometry(compchem::Molecule **mol, int argc, char **argv);

void initialize(compchem::Molecule **mol, int argc, char **argv);

}

#endif /* MAIN_HPP_ */
