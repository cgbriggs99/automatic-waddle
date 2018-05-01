/*
 * calculations1.cpp
 *
 *  Created on: Apr 27, 2018
 *      Author: Connor
 */

#ifndef CALCULATIONS1_CPP
#define CALCULATIONS1_CPP

#include "../compchem.hpp"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;
using namespace compchem;
using namespace array;

static int compare(const void *a, const void *b) {
	if(*((double *) a) - *((double *) b) > 0) {
		return 1;
	} else if(*((double *) a) - *((double *) b) < 0) {
		return -1;
	}
	return (0);
}

//Simple 3d distance calculation without overflow or underflow.
static double hypot3(double x, double y, double z) {
	double max, a, b;

	if(fabs(x) >= fabs(y) && fabs(x) >= fabs(z)) {
		max = x;
		if(fabs(y) > fabs(z)) {
			a = y;
			b = z;
		} else {
			a = z;
			b = y;
		}
	} else if(fabs(y) > fabs(x) && fabs(y) >= fabs(z)) {
		max = y;
		if(fabs(x) > fabs(z)) {
			a = x;
			b = z;
		} else {
			a = z;
			b = x;
		}
	} else {
		max = z;
		if(fabs(x) > fabs(y)) {
			a = x;
			b = y;
		} else {
			a = y;
			b = x;
		}
	}

	if (max == 0) {
		return (0);
	}

	double norm1, norm2;
	norm1 = a / max;
	norm2 = b / max;
	return (fabs(max) * sqrt(1 + norm1 * norm1 + norm2 * norm2));
}

static double hypot3(Vector<double> v) {
	return (hypot3(v[0], v[1], v[2]));
}

void compchem::Molecule::computeDists() {
	for (int i = 0; i < this->numatoms; i++) {
		(*(this->distances))(i, i) = 0;
		for (int j = 0; j < i; j++) {
			(*(this->distances))(i, j) = hypot3(
					this->atoms[i].pos[0] - this->atoms[j].pos[0],
					this->atoms[i].pos[1] - this->atoms[j].pos[1],
					this->atoms[i].pos[2] - this->atoms[j].pos[2]);
			(*(this->distances))(j, i) = (*(this->distances))(i, j);
		}
	}
}

void compchem::Molecule::computeAngles() {
	for (int i = 0; i < this->numatoms; i++) {
		for (int j = 0; j < this->numatoms; j++) {
			for (int k = 0; k < this->numatoms; k++) {
				if (i == j || i == k || j == k) {
					(*(this->angles))(i, j, k) = 0;
					continue;
				}

				Vector<double> d1 = this->atoms[j].pos - this->atoms[i].pos,
						d2 = this->atoms[k].pos - this->atoms[i].pos;

				(*(this->angles))(i, j, k) = acos(
						(d1 * d2) / (hypot3(d1) * hypot3(d2)));
			}
		}
	}
}

void compchem::Molecule::computePlaneAngles() {
	for (int i = 0; i < this->numatoms; i++) {
		for (int j = 0; j < this->numatoms; j++) {
			for (int k = 0; k < this->numatoms; k++) {
				for (int l = 0; l < this->numatoms; l++) {
					Vector<double> d1 = this->atoms[j].pos - this->atoms[i].pos,
							d2 = this->atoms[k].pos - this->atoms[i].pos, d3 =
									this->atoms[l].pos - this->atoms[i].pos;
					(*(this->plane_angles))(i, j, k, l) = ((d1 % d2)
							/ sin((*(this->angles))(i, j, k))) * d3;
				}
			}
		}
	}
}

void compchem::Molecule::computeTorsionAngles() {
	for (int i = 0; i < this->numatoms; i++) {
		for (int j = 0; j < this->numatoms; j++) {
			for (int k = 0; k < this->numatoms; k++) {
				for (int l = 0; l < this->numatoms; l++) {
					Vector<double> d1 = this->atoms[i].pos - this->atoms[j].pos,
							d2 = this->atoms[j].pos - this->atoms[k].pos, d3 =
									this->atoms[l].pos - this->atoms[k].pos;
					(*(this->torsion_angles))(i, j, k, l) = acos(
							((d1 % d2) * (d2 % d3))
									/ (sin((*(this->angles))(i, j, k))
											* sin((*(this->angles))(j, k, l))));
				}
			}
		}
	}
}

void compchem::Molecule::translateCOM() {
	double mass = 0;

	for (int i = 0; i < this->numatoms; i++) {
		try {
			this->center_of_mass += this->atoms[i].mass * this->atoms[i].pos;
			mass += this->atoms[i].mass;
		} catch (array::SizeMismatchException &e) {
			puts(e.what());
			exit(-1);
		}
	}

	this->center_of_mass /= mass;

	for (int i = 0; i < this->numatoms; i++) {
		this->atoms[i].pos -= this->center_of_mass;
	}
}

void compchem::Molecule::momentsOfInertia() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == j) {
				this->inertial_moments(i, i) = 0;
				for (int k = 0; k < this->numatoms; k++) {
					this->inertial_moments(i, i) += this->atoms[k].mass
							* (this->atoms[k].pos[(i + 1) % 3]
									* this->atoms[k].pos[(i + 1) % 3]
									+ this->atoms[k].pos[(i + 2) % 3]
											* this->atoms[k].pos[(i + 2) % 3]);
				}
			} else {
				this->inertial_moments(i, j) = 0;
				for (int k = 0; k < this->numatoms; k++) {
					this->inertial_moments(i, j) += this->atoms[k].mass
							* this->atoms[k].pos[i] * this->atoms[k].pos[j];
				}
			}
		}
	}

	double *im = (double *) malloc(3 * sizeof(double));

	LAPACKE_dgeev(LAPACK_ROW_MAJOR, 'N', 'N', 3,
			this->inertial_moments.getArray(), 3,
			this->principle_moments.getArray(), im, NULL, 3, NULL, 3);
	qsort(this->principle_moments.getArray(), 3, sizeof(double), compare);
}

//Defines constants that will be necessary for calculations.
#define PLANCK 3.9905e31 //amu angstrom^2 / s
#define C 2997924580000000000	//angstrom / s

void compchem::Molecule::rotations() {
	this->rotational_constants(0) = PLANCK
			/ (8 * M_PI * M_PI * C * this->principle_moments(0) * .2800285)
		    / 10000000000;
	this->rotational_constants(1) = PLANCK
		    / (8 * M_PI * M_PI * C * this->principle_moments(1) * .2800285)
		    / 10000000000;
	this->rotational_constants(2) = PLANCK
		    / (8 * M_PI * M_PI * C * this->principle_moments(2) * .2800285)
		    / 10000000000;
}
#endif
