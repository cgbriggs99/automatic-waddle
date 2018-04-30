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
#include "../input.hpp"
#include "../output.hpp"
#include "../arrays.hpp"
#include "../molecule.hpp"

namespace compchem {

void initialize(Molecule **mol, int argc, char **argv);

}

#endif /* MAIN_HPP_ */
