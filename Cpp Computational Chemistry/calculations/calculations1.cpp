/*
 * calculations1.cpp
 *
 *  Created on: Apr 27, 2018
 *      Author: Connor
 */

#include "../output.hpp"
#include "../arrays.hpp"
#include "../input.hpp"
#include "../molecule.hpp"
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
	double x1 = x, y1 = y;

	//Simple mergesort.
	if (fabs(x1) < fabs(y1)) {
		a = x1;
		x1 = y1;
		y1 = a;
	}
	if (fabs(z) < fabs(x1)) {
		a = z;
		b = x1;
		max = y1;
	} else if (fabs(z) < fabs(y1)) {
		a = x1;
		b = z;
		max = y1;
	} else {
		a = x1;
		b = y1;
		max = z;
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

void Molecule::computeDists() {
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

void Molecule::computeAngles() {
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

void Molecule::computePlaneAngles() {
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

void Molecule::computeTorsionAngles() {
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

void Molecule::translateCOM() {
	Vector<double> center_of_mass(3);
	double mass = 0;

	for (int i = 0; i < this->numatoms; i++) {
		center_of_mass += this->atoms[i].mass * this->atoms[i].pos;
		mass += this->atoms[i].mass;
	}

	center_of_mass /= mass;

	for (int i = 0; i < this->numatoms; i++) {
		this->atoms[i].pos -= center_of_mass;
	}
}

void Molecule::momentsOfInertia() {
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
			this->principle_moments.getArray(), im, NULL, 1, NULL, 1);
	qsort(this->principle_moments.getArray(), 3, sizeof(double), compare);
}

void Molecule::rotations() {
	//TODO transform the constants between systems.
}
