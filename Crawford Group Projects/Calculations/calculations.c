/* calculations.c
 * Does various calculations concerning molecular geometry.
 * Copyright (C) ${year} Connor Briggs
 */
/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/*
 * I can be reached at cgbriggs99@gmail.com if you have any questions,
 * concerns, or bugs.
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <complex.h>
#include <stdio.h>
#include "../calculations.h"
#include "../molecule.h"
#include "../output.h"

static double dotProd(double x1, double y1, double z1, double x2, double y2,
                      double z2) {
	return (x1 * x2 + y1 * y2 + z1 * z2);
}

static double crossX(double y1, double y2, double z1, double z2) {
	return (y1 * z2 - y2 * z1);
}

#define X(atom) molecule->atoms[(atom)].pos[0]
#define Y(atom) molecule->atoms[(atom)].pos[1]
#define Z(atom) molecule->atoms[(atom)].pos[2]
#define D(atom1, atom2) molecule->distances[(atom1)][(atom2)]

void distances(molecule_t *molecule) {
	for(int i = 0; i < molecule->num_atoms; i++) {
		for(int j = 0; j < molecule->num_atoms; j++) {
			if(i == j) {
				molecule->distances[i][j] = 0;
				continue;
			}
			molecule->distances[i][j] = hypot(
			    molecule->atoms[i].pos[0] - molecule->atoms[j].pos[0],
			    hypot(molecule->atoms[i].pos[1] - molecule->atoms[j].pos[1],
			        molecule->atoms[i].pos[2] - molecule->atoms[j].pos[2]));
		}
	}
}

void angles(molecule_t *molecule) {
	for(int i = 0; i < molecule->num_atoms; i++) {
		for(int j = 0; j < molecule->num_atoms; j++) {
			for(int k = 0; k < molecule->num_atoms; k++) {
				if(k == i || k == j || j == i) {
					molecule->angles[i][j][k] = 0;
					continue;
				}
				molecule->angles[i][j][k] = acos(dotProd((X(i)- X(j)) / D(i, j),
				(Y(i) - Y(j)) / D(i, j),
				(Z(i) - Z(j)) / D(i, j), (X(k) - X(j)) / D(j, k),
				(Y(k) - Y(j)) / D(j, k), (Z(k) - Z(j)) / D(j, k)));
			}
		}
	}
}

void planeAngles(molecule_t *molecule) {
	for(int i = 0; i < molecule->num_atoms; i++) {
		for(int j = 0; j < molecule->num_atoms; j++) {
			for(int k = 0; k < molecule->num_atoms; k++) {
				for(int l = 0; l < molecule->num_atoms; l++) {
					if(i == j || i == k || i == l || j == k || j == l
					    || k == l) {
						molecule->plane_angles[i][j][k][l] = 0;
						continue;
					} else if(sin(molecule->angles[j][k][l]) == 0) {
						molecule->plane_angles[i][j][k][l] = M_PI;
					} else {
						molecule->plane_angles[i][j][k][l] = asin(-(X(j)* Y(l) * Z(i) + X(l) * Y(i) * Z(j) +
						X(i) * Y(j) * Z(l) + X(j) * Y(k) * Z(i) + X(k) * Y(i) * Z(j) + X(i) * Y(j) * Z(k) +
						X(l) * Y(k) * Z(i) + X(k) * Y(i) * Z(l) + X(i) * Y(l) * Z(k) - X(j) * Y(l) * Z(k) +
						X(j) * Y(k) * Z(l) + X(l) * Y(j) * Z(k) + X(k) * Y(l) * Z(j) - X(j) * Y(i) * Z(l) -
						X(l) * Y(j) * Z(i) - X(i) * Y(l) * Z(j) - X(j) * Y(i) * Z(k) - X(k) * Y(j) * Z(i) -
						X(i) * Y(k) * Z(j) - X(l) * Y(i) * Z(k) - X(k) * Y(l) * Z(i) - X(i) * Y(k) * Z(l) -
						X(l) * Y(k) * Z(j) - X(k) * Y(j) * Z(l))
						/ (sin(molecule->angles[j][k][l]) * D(j, k) * D(k, l) * D(i, k)));
					}
				}
			}
		}
	}
}

void torsionAngles(molecule_t *molecule) {
	for(int i = 0; i < molecule->num_atoms; i++) {
		for(int j = 0; j < molecule->num_atoms; j++) {
			for(int k = 0; k < molecule->num_atoms; k++) {
				for(int l = 0; l < molecule->num_atoms; l++) {
					if(i == j || i == k || i == l || j == k || j == l
					    || k == l) {
						molecule->torsion_angles[i][j][k][l] = 0;
						continue;
					} else if(sin(molecule->angles[i][j][k]) == 0
					    || sin(molecule->angles[j][k][l]) == 0) {
						molecule->torsion_angles[i][j][k][l] = M_PI;
					} else {
						double ax = (X(j)- X(i)) / D(i, j), ay = (Y(j) - Y(i)) / D(i, j),
						az = (Z(j) - Z(i)) / D(i, j);
						double bx = (X(k)- X(j)) / D(j, k), by = (Y(k) - Y(j)) / D(k, j),
						bz = (Z(k) - Z(j)) / D(j, k);
						double cx = (X(l)- X(k)) / D(k, l), cy = (Y(l) - Y(k)) / D(k, l),
						cz = (Z(l) - Z(k)) / D(k, l);
						molecule->torsion_angles[i][j][k][l] = acos(
						(ay * by * bz * cz - az * by * by * cz
								- ay * bz * bz * cy + az * by * bz * cy
								- ax * bz * bx * cz - az * bx * bx * cz
								- ax * bz * bz * cx + az * bx * bz * cx
								+ ax * by * bx * cy - ay * bx * bx * cy
								- ax * by * by * cx + ay * bx * by * cx)
						/ (sin(molecule->angles[i][j][k])
								* sin(molecule->angles[j][k][l])));
					}
					if(molecule->torsion_angles[i][j][k][l] == sqrt(-1)) {
						molecule->torsion_angles[i][j][k][j] = M_PI;
					}
				}
			}
		}
	}
}

void centerOfMass(molecule_t *molecule) {
	double mx = 0, my = 0, mz = 0;
	for(int i = 0; i < molecule->num_atoms; i++) {
		mx += molecule->atoms[i].mass * molecule->atoms[i].pos[0];
		my += molecule->atoms[i].mass * molecule->atoms[i].pos[1];
		mz += molecule->atoms[i].mass * molecule->atoms[i].pos[2];
	}

	molecule->center_of_mass[0] = mx / molecule->mass;
	molecule->center_of_mass[1] = my / molecule->mass;
	molecule->center_of_mass[2] = mz / molecule->mass;
}

void inertialMoments(molecule_t *molecule) {
//Initialize for summing
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			molecule->inertial_moments[i][j] = 0;
		}
	}

	for(int i = 0; i < molecule->num_atoms; i++) {
		molecule->inertial_moments[0][0] += molecule->atoms[i].mass * (Y(i)* Y(i) + Z(i) * Z(i));
		molecule->inertial_moments[1][1] += molecule->atoms[i].mass * (X(i) * X(i) + Z(i) * Z(i));
		molecule->inertial_moments[2][2] += molecule->atoms[i].mass * (X(i) * X(i) + Y(i) * Y(i));
		molecule->inertial_moments[1][0] += molecule->atoms[i].mass * X(i) * Y(i);
		molecule->inertial_moments[2][0] += molecule->atoms[i].mass * X(i) * Z(i);
		molecule->inertial_moments[1][2] += molecule->atoms[i].mass * Z(i) * Y(i);
	}
	molecule->inertial_moments[0][1] = molecule->inertial_moments[1][0];
	molecule->inertial_moments[0][2] = molecule->inertial_moments[2][0];
	molecule->inertial_moments[2][1] = molecule->inertial_moments[1][2];
}

void principalMoments(molecule_t *molecule) {
	int compare(const void *a, const void *b) {
		if(*(double *) a - *(double *) b > 0) {
			return (1);
		} else if(*(double *) a - *(double *) b == 0) {
			return (0);
		} else {
			return (-1);
		}
	}

	/*
	 * Algorithm from Wikipedia.
	 */
	double p1 = molecule->inertial_moments[0][1]
	    * molecule->inertial_moments[0][1]
	    + molecule->inertial_moments[0][2] * molecule->inertial_moments[0][2]
	    + molecule->inertial_moments[1][2] * molecule->inertial_moments[1][2];
	if(p1 == 0) {
		molecule->principal_moments[0] = molecule->inertial_moments[0][0];
		molecule->principal_moments[1] = molecule->inertial_moments[1][1];
		molecule->principal_moments[2] = molecule->inertial_moments[2][2];
		qsort(molecule->principal_moments, 3, sizeof(double), compare);
	} else {
		double q = (molecule->inertial_moments[0][0]
		    + molecule->inertial_moments[1][1]
		    + molecule->inertial_moments[2][2]) / 3;
		double p2 = (molecule->inertial_moments[0][0] - q)
		    * (molecule->inertial_moments[0][0] - q)
		    + (molecule->inertial_moments[1][1] - q)
		        * (molecule->inertial_moments[1][1] - q)
		    + (molecule->inertial_moments[2][2] - q)
		        * (molecule->inertial_moments[2][2] - q) + 2 * p1;
		double p = sqrt(p2 / 6);
		double B[3][3];
		for(int i = 0; i < 3; i++) {
			for(int j = 0; j < 3; j++) {
				if(i == j) {
					B[i][j] = (1 / p) * (molecule->inertial_moments[i][j] - q);
				} else {
					B[i][j] = (1 / p) * molecule->inertial_moments[i][j];
				}
			}
		}

		double r = (B[0][0] * B[1][1] * B[2][2] + B[0][1] * B[1][2] * B[2][0]
		    + B[0][2] * B[1][0] * B[2][1] - B[0][0] * B[1][2] * B[2][1]
		    - B[0][1] * B[1][0] * B[2][2] - B[0][2] * B[1][1] * B[2][0]) / 2;
		double phi;

		if(r <= -1) {
			phi = M_PI / 3;
		} else if(r >= 1) {
			phi = 0;
		} else {
			phi = acos(r) / 3;
		}

		molecule->principal_moments[2] = q + 2 * p * cos(phi);
		molecule->principal_moments[0] = q + 2 * p * cos(phi + (2 * M_PI / 3));
		molecule->principal_moments[1] = 3 * q - molecule->principal_moments[0]
		    - molecule->principal_moments[2];
	}
}

void rotationalConstants(molecule_t *molecule) {
	molecule->rotational_constants[0] = PLANCK
	    / (8 * M_PI * M_PI * C * molecule->principal_moments[0] * .2800285)
	    / 10000000000;
	molecule->rotational_constants[1] = PLANCK
	    / (8 * M_PI * M_PI * C * molecule->principal_moments[1] * .2800285)
	    / 10000000000;
	molecule->rotational_constants[2] = PLANCK
	    / (8 * M_PI * M_PI * C * molecule->principal_moments[2] * .2800285)
	    / 10000000000;
}

void center(molecule_t *molecule) {
	for(int i = 0; i < molecule->num_atoms; i++) {
		molecule->atoms[i].pos[0] -= molecule->center_of_mass[0];
		molecule->atoms[i].pos[1] -= molecule->center_of_mass[1];
		molecule->atoms[i].pos[2] -= molecule->center_of_mass[2];
	}
}

void rotor(molecule_t *molecule) {
	if(((float) molecule->principal_moments[0]
	    == (float) molecule->principal_moments[1])
	    && ((float) molecule->principal_moments[1]
	        == (float) molecule->principal_moments[2])) {
		molecule->rotor = SPHERICAL_TOP;
	} else if(((float) molecule->principal_moments[0]
	    < (float) molecule->principal_moments[1])
	    && ((float) molecule->principal_moments[1]
	        == (float) molecule->principal_moments[2])) {
		molecule->rotor = LINEAR;
	} else if(((float) molecule->principal_moments[0]
	    == (float) molecule->principal_moments[1])
	    || ((float) molecule->principal_moments[1]
	        == (float) molecule->principal_moments[2])) {
		molecule->rotor = SYMMETRIC;
	} else {
		molecule->rotor = ASYMMETRIC;
	}
}

#undef X
#undef Y
#undef Z

void weightHessian(molecule_t *molecule) {
	for(int i = 0; i < molecule->num_hessian; i++) {
		for(int j = 0; j < molecule->num_hessian; j++) {
			molecule->hessian[i][j] /= sqrt(
			    molecule->atoms[(i) / 3].mass * molecule->atoms[j / 3].mass);
		}
	}
}

static int compare(const void *a, const void *b) {
	if(*(double *) a <= *(double *) b) {
		return (-1);
	} else {
		return (1);
	}
}

void eigensHessian(molecule_t *molecule) {
#define EPS 0.000000001
	/*
	 * Calculate eigenvalues.
	 * Step 1: Decompose matrix into QR form.
	 * Step 2: Create new matrix RQ.
	 * Step 3: Repeat.
	 */
	double *mat, *im, *r, *vl, *vr;
	int n = molecule->num_hessian;
	double check1, check2 = 0;
	unsigned int count = 0;

	mat = calloc(n * n, sizeof(double));
	im = calloc(n, sizeof(double));
	r = calloc(n, sizeof(double));
	vl = calloc(n * n, sizeof(double));
	vr = calloc(n * n, sizeof(double));

//#pragma acc
	for(int i = 0; i < n; i++) {
		memcpy(mat + n * i, molecule->hessian[i], n * sizeof(double));
	}

	LAPACKE_dgeev(LAPACK_ROW_MAJOR, 'N', 'N', n, mat, n, r, im, vl, 1, vr, 1);

	for(int i = 0; i < n; i++) {
		molecule->hessian_eigs[i] = r[i];
	}

	free(mat);
	free(im);
	free(r);
#ifdef OPTIMIZE
	free(rot);
#endif
}

/*
 * Calculate the core hamiltonian.
 */
void hamiltonian(molecule_t *molecule) {
	for(int i = 0; i < molecule->orbitals; i++) {
		for(int j = 0; j < molecule->orbitals; j++) {
			molecule->hamiltonian[i][j] = molecule->kin_energy[i][j]
			    + molecule->attraction[i][j];
		}
	}
}

/*
 * Find the inverse square root of the molecule's overlap matrix.
 */
void sqrtMolecule(molecule_t *molecule) {
	double **mat, **er, **evr, **evl, **temp, **work;
	int n = molecule->orbitals, *piv;
	mat = malloc(molecule->orbitals * sizeof(double *));
	evl = malloc(molecule->orbitals * sizeof(double *));
	temp = malloc(molecule->orbitals * sizeof(double *));
	evr = calloc(n, sizeof(double *));
	work = calloc(n, sizeof(double *));
	piv = malloc(n * n * sizeof(int));

	temp[0] = malloc(n * n * sizeof(double));
	mat[0] = malloc(n * n * sizeof(double));
	er = malloc(molecule->orbitals * sizeof(double *));
	*evr = malloc(molecule->orbitals * n * sizeof(double));
	evl[0] = malloc(n * n * sizeof(double));
	work[0] = calloc(n * n, sizeof(double));
	*er = calloc(n * n, sizeof(double));

	for(int i = 1; i < molecule->orbitals; i++) {
		mat[i] = mat[i - 1] + molecule->orbitals;
		evl[i] = evl[i - 1] + molecule->orbitals;
		temp[i] = temp[i - 1] + molecule->orbitals;
		evr[i] = evr[i - 1] + n;
		work[i] = work[i - 1] + n;
		er[i] = er[i - 1] + n;
	}

	for(int i = 0; i < molecule->orbitals; i++) {
		for(int j = 0; j < molecule->orbitals; j++) {
			double x = molecule->overlap[i][j];
			mat[i][j] = x;
		}
	}

	LAPACKE_dgesvd(LAPACK_ROW_MAJOR, 'A', 'A', molecule->orbitals, n, *mat,
	    molecule->orbitals, *er, *evl, n, *evr, molecule->orbitals, *temp);

	for(int i = 0; i < molecule->orbitals; i++) {
		for(int j = 0; j < molecule->orbitals; j++) {
			if(i == j) {
				mat[i][j] = 1 / sqrt(er[0][i]);
			} else {
				mat[i][j] = 0;
			}
		}
	}

	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, molecule->orbitals,
	    molecule->orbitals, molecule->orbitals, 1.0, *evl, molecule->orbitals,
	    *mat, molecule->orbitals, 0, *work, molecule->orbitals);

	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans, molecule->orbitals,
	    molecule->orbitals, molecule->orbitals, 1.0, *work, molecule->orbitals,
	    *evl, molecule->orbitals, 0, *mat, molecule->orbitals);

	for(int i = 0; i < molecule->orbitals; i++) {
		for(int j = 0; j < molecule->orbitals; j++) {
			molecule->symmetric[i][j] = mat[i][j];
			molecule->symm_trans[i][j] = mat[j][i];
		}
	}

	free(mat[0]);
	free(evl[0]);
	free(temp[0]);
	free(evr[0]);
	free(work[0]);
	free(work);
	free(evr);
	free(temp);
	free(mat);
	free(evl);
	free(er);
}

static int comparedd(const void *a, const void *b) {
	if(**((double **) a) > **((double **) b)) {
		return (1);
	} else {
		return (-1);
	}
}

void findFock(molecule_t *molecule) {
#undef EPS
#undef DEL
#define EPS 0.000000000001
#define DEL 0.000000000001

	hamiltonian(molecule);
	sqrtMolecule(molecule);

	int n = molecule->orbitals;	//So that the same thing does not need to be typed repeatedly.
	int count = 0;

	double elec, energy = 0, elast, rms;

	double **f0, **f1, **c0, **c1, **d0, **d1, **work1, **work2, **work3, **ham,
	        **shalf;

	double **sort;

	f0 = calloc(n, sizeof(double *));
	f1 = calloc(n, sizeof(double *));
	c0 = calloc(n, sizeof(double *));
	c1 = calloc(n, sizeof(double *));
	d0 = calloc(n, sizeof(double *));
	d1 = calloc(n, sizeof(double *));
	work1 = calloc(n, sizeof(double *));
	work2 = calloc(n, sizeof(double *));
	work3 = calloc(n, sizeof(double *));
	ham = calloc(n, sizeof(double *));
	shalf = calloc(n, sizeof(double *));

	*f0 = calloc(n * n, sizeof(double));
	*f1 = calloc(n * n, sizeof(double));
	*c0 = calloc(n * n, sizeof(double));
	*c1 = calloc(n * n, sizeof(double));
	*d0 = calloc(n * n, sizeof(double));
	*d1 = calloc(n * n, sizeof(double));
	*work1 = calloc(n * n, sizeof(double));
	*work2 = calloc(n * n, sizeof(double));
	*work3 = calloc(n * n, sizeof(double));
	*ham = calloc(n * n, sizeof(double));
	*shalf = calloc(n * n, sizeof(double));

	sort = calloc(n, sizeof(double *));

	for(int i = 1; i < n; i++) {
		f0[i] = *f0 + n * i;
		f1[i] = *f1 + n * i;
		c0[i] = *c0 + n * i;
		c1[i] = *c1 + n * i;
		d0[i] = *d0 + n * i;
		d1[i] = *d1 + n * i;
		work1[i] = *work1 + n * i;
		work2[i] = *work2 + n * i;
		work3[i] = *work3 + n * i;
		ham[i] = *ham + n * i;
		shalf[i] = *shalf + n * i;
	}

	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			shalf[i][j] = molecule->symmetric[i][j];
		}
	}

	do {
		elast = energy;
		if(count == 0) {
			for(int i = 0; i < n; i++) {
				for(int j = 0; j < n; j++) {
					//Find the initial Fock guess.
					f0[i][j] = ham[i][j] = molecule->hamiltonian[i][j];
				}
			}
		} else {
			memcpy(*d1, *d0, n * n * sizeof(double));
			for(int i = 0; i < n; i++) {
				for(int j = 0; j < n; j++) {
					f0[i][j] = ham[i][j];
					for(int k = 0; k < n; k++) {
						for(int l = 0; l < n; l++) {
							f0[i][j] += d0[k][l]
							    * (2 * molecule->two_electron[TEI(i, j, k, l)]
							        - molecule->two_electron[TEI(i, k, j, l)]);
						}
					}
				}
			}
		}

		cblas_dgemm(CblasRowMajor, CblasTrans, CblasNoTrans, n, n, n, 1.0,
		    *shalf, n, *f0, n, 0, *work1, n);
		cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0,
		    *work1, n, *shalf, n, 0, *f1, n);

		memset(work1[0], 0, n * n * sizeof(double));
		memset(work2[0], 0, n * n * sizeof(double));
		memset(work3[0], 0, n * n * sizeof(double));

		LAPACKE_dgeev(LAPACK_ROW_MAJOR, 'N', 'V', n, *f1, n, *work1, *work2,
		    *work3, n, *c1, n);

		//Prepare for sorting.

		for(int i = 0; i < n; i++) {
			for(int j = 0; j < n; j++) {
				work2[i][j] = c1[i][j];
			}
		}

		//Sort
		for(int i = 0; i < n; i++) {
			sort[i] = work1[0] + i;
		}

		qsort(sort, n, sizeof(double *), comparedd);

		//Sift through data.
		for(int i = 0; i < n; i++) {
			unsigned long off = ((unsigned long) sort[i]
			    - (unsigned long) work1[0]);
			off /= sizeof(double);
			for(int j = 0; j < n; j++) {
				c1[j][i] = work2[j][off];
			}

		}

		cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0,
		    *shalf, n, *c1, n, 0, *c0, n);

		for(int i = 0; i < n; i++) {
			for(int j = 0; j < n; j++) {
				d0[i][j] = 0;
				for(int k = 0; k < molecule->electrons / 2; k++) {
					d0[i][j] += c0[i][k] * c0[j][k];
				}
			}
		}

		elec = 0;
		for(int i = 0; i < n; i++) {
			for(int j = 0; j < n; j++) {
				elec += d0[i][j] * (ham[i][j] + f0[i][j]);
			}
		}
		energy = elec + molecule->enuc;
		rms = 0;
		for(int i = 0; i < n; i++) {
			for(int j = 0; j < n; j++) {
				rms += (d0[i][j] - d1[i][j]) * (d0[i][j] - d1[i][j]);
			}
		}
		rms = sqrt(rms);

		count++;
	} while(count < 100 && (fabs(elast - energy) > EPS && rms > DEL));

	molecule->scf_energy = energy;

	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			molecule->density[i][j] = d0[i][j];
			molecule->fock[i][j] = f0[i][j];
			molecule->molecular_orbitals[i][j] = c0[i][j];
			molecule->molecular_eigs[i][j] = ((i == j)? sort[i][0]: 0);
		}
	}

	free(*c0);
	free(*c1);
	free(*d0);
	free(*d1);
	free(*f0);
	free(*f1);
	free(*ham);
	free(*shalf);
	free(*work1);
	free(*work2);
	free(*work3);
	free(sort);
	free(c0);
	free(c1);
	free(d0);
	free(d1);
	free(f0);
	free(f1);
	free(ham);
	free(shalf);
	free(work1);
	free(work2);
	free(work3);
}

/*
 * Calculate the self-consistent fock matrix.
 */
void findFockPrint(molecule_t *molecule) {

	hamiltonian(molecule);
	sqrtMolecule(molecule);

	int n = molecule->orbitals;	//So that the same thing does not need to be typed repeatedly.
	int count = 0;

	double elec, energy = 0, elast, rms;

	double **f0, **f1, **c0, **c1, **d0, **d1, **work1, **work2, **work3, **ham,
	        **shalf;

	double **sort;

	f0 = calloc(n, sizeof(double *));
	f1 = calloc(n, sizeof(double *));
	c0 = calloc(n, sizeof(double *));
	c1 = calloc(n, sizeof(double *));
	d0 = calloc(n, sizeof(double *));
	d1 = calloc(n, sizeof(double *));
	work1 = calloc(n, sizeof(double *));
	work2 = calloc(n, sizeof(double *));
	work3 = calloc(n, sizeof(double *));
	ham = calloc(n, sizeof(double *));
	shalf = calloc(n, sizeof(double *));

	*f0 = calloc(n * n, sizeof(double));
	*f1 = calloc(n * n, sizeof(double));
	*c0 = calloc(n * n, sizeof(double));
	*c1 = calloc(n * n, sizeof(double));
	*d0 = calloc(n * n, sizeof(double));
	*d1 = calloc(n * n, sizeof(double));
	*work1 = calloc(n * n, sizeof(double));
	*work2 = calloc(n * n, sizeof(double));
	*work3 = calloc(n * n, sizeof(double));
	*ham = calloc(n * n, sizeof(double));
	*shalf = calloc(n * n, sizeof(double));

	sort = calloc(n, sizeof(double *));

	for(int i = 1; i < n; i++) {
		f0[i] = *f0 + n * i;
		f1[i] = *f1 + n * i;
		c0[i] = *c0 + n * i;
		c1[i] = *c1 + n * i;
		d0[i] = *d0 + n * i;
		d1[i] = *d1 + n * i;
		work1[i] = *work1 + n * i;
		work2[i] = *work2 + n * i;
		work3[i] = *work3 + n * i;
		ham[i] = *ham + n * i;
		shalf[i] = *shalf + n * i;
	}

	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			shalf[i][j] = molecule->symmetric[i][j];
		}
	}

	do {
		elast = energy;
		if(count == 0) {
			printf("\nInitial Fock matrix\n");
			for(int i = 0; i < n; i++) {
				for(int j = 0; j < n; j++) {
					//Find the initial Fock guess.
					f0[i][j] = ham[i][j] = molecule->hamiltonian[i][j];
					printf("%f\t", f0[i][j]);
				}
				printf("\n");
			}
		} else {
			memcpy(*d1, *d0, n * n * sizeof(double));
			for(int i = 0; i < n; i++) {
				for(int j = 0; j < n; j++) {
					f0[i][j] = ham[i][j];
					for(int k = 0; k < n; k++) {
						for(int l = 0; l < n; l++) {
							f0[i][j] += d0[k][l]
							    * (2 * molecule->two_electron[TEI(i, j, k, l)]
							        - molecule->two_electron[TEI(i, k, j, l)]);
						}
					}
				}
			}
		}

		cblas_dgemm(CblasRowMajor, CblasTrans, CblasNoTrans, n, n, n, 1.0,
		    *shalf, n, *f0, n, 0, *work1, n);
		cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0,
		    *work1, n, *shalf, n, 0, *f1, n);

		if(count == 0) {
			printArray("\nInitial F'\n", f1, n, n);
			printf("\nElec\tEnergy\n");
		}
		if(count == 1) {
			printf("\nFock matrix\n");
			for(int i = 0; i < n; i++) {
				for(int j = 0; j < n; j++) {
					printf("%f\t", f0[i][j]);
				}
				printf("\n");
			}
			printf("\nElec\t\tEnergy\t\tDiff\t\tRMS\n");
		}

		memset(work1[0], 0, n * n * sizeof(double));
		memset(work2[0], 0, n * n * sizeof(double));
		memset(work3[0], 0, n * n * sizeof(double));

		LAPACKE_dgeev(LAPACK_ROW_MAJOR, 'N', 'V', n, *f1, n, *work1, *work2,
		    *work3, n, *c1, n);

		//Prepare for sorting.

		for(int i = 0; i < n; i++) {
			for(int j = 0; j < n; j++) {
				work2[i][j] = c1[i][j];
			}
		}

		//Sort
		for(int i = 0; i < n; i++) {
			sort[i] = work1[0] + i;
		}

		qsort(sort, n, sizeof(double *), comparedd);

		//Sift through data.
		for(int i = 0; i < n; i++) {
			unsigned long off = ((unsigned long) sort[i]
			    - (unsigned long) work1[0]);
			off /= sizeof(double);
			for(int j = 0; j < n; j++) {
				c1[j][i] = work2[j][off];
			}

		}

		cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0,
		    *shalf, n, *c1, n, 0, *c0, n);

		for(int i = 0; i < n; i++) {
			for(int j = 0; j < n; j++) {
				d0[i][j] = 0;
				for(int k = 0; k < molecule->electrons / 2; k++) {
					d0[i][j] += c0[i][k] * c0[j][k];
				}
			}
		}
		if(count == 0) {
			printArray("\nInitial C0\n", c0, n, n);
			printArray("\nInitial Density\n", d0, n, n);
			printf("\nElectric\tEnergy\n");
		}

		elec = 0;
		for(int i = 0; i < n; i++) {
			for(int j = 0; j < n; j++) {
				elec += d0[i][j] * (ham[i][j] + f0[i][j]);
			}
		}
		energy = elec + molecule->enuc;
		rms = 0;
		for(int i = 0; i < n; i++) {
			for(int j = 0; j < n; j++) {
				rms += (d0[i][j] - d1[i][j]) * (d0[i][j] - d1[i][j]);
			}
		}
		rms = sqrt(rms);

		printf("%.17f\t%.17f\t%.17f\t%.17f\n", elec, energy, fabs(elast - energy), rms);
		count++;
	} while(count < 100 && (fabs(elast - energy) > EPS && rms > DEL));

	molecule->scf_energy = energy;

	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			molecule->density[i][j] = d0[i][j];
			molecule->fock[i][j] = f0[i][j];
			molecule->molecular_orbitals[i][j] = c0[i][j];
			molecule->molecular_eigs[i][j] = (i == j)? *(sort[i]): 0;
		}
	}

	free(*c0);
	free(*c1);
	free(*d0);
	free(*d1);
	free(*f0);
	free(*f1);
	free(*ham);
	free(*shalf);
	free(*work1);
	free(*work2);
	free(*work3);
	free(sort);
	free(c0);
	free(c1);
	free(d0);
	free(d1);
	free(f0);
	free(f1);
	free(ham);
	free(shalf);
	free(work1);
	free(work2);
	free(work3);
}

/*
 * Find the charges and dipole moment.
 */
void calculateCharges(molecule_t *molecule) {
	double **mat, **s, **d, *er, *ei;
	int j = 0, n = molecule->orbitals;

	mat = calloc(n, sizeof(double *));
	s = calloc(n, sizeof(double *));
	d = calloc(n, sizeof(double *));
	er = calloc(n, sizeof(double));
	ei = calloc(n, sizeof(double));

	*mat = calloc(n * n, sizeof(double));
	*s = calloc(n * n, sizeof(double));
	*d = calloc(n * n, sizeof(double));
	for(int i = 1; i < n; i++) {
		mat[i] = mat[i - 1] + n;
		s[i] = s[i - 1] + n;
		d[i] = d[i - 1] + n;
	}

	for(int i = 0; i < n; i++) {
		for(int k = 0; k < n; k++) {
			s[i][k] = molecule->overlap[i][k];
			d[i][k] = molecule->density[i][k];
		}
	}

	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0, *d, n,
	    *s, n, 0, *mat, n);

	for(int i = 0; i < molecule->num_atoms; i++) {
		molecule->atoms[i].true_charge = molecule->atoms[i].z;
		for(int k = j; k < j + orbitals(molecule->atoms[i].z); k++) {
			molecule->atoms[i].true_charge -= 2 * mat[k][k];
		}
		j += orbitals(molecule->atoms[i].z);
	}

	LAPACKE_dgeev(LAPACK_ROW_MAJOR, 'N', 'V', n, *s, n, er, ei, NULL, n, *mat,
	    n);

	memset(*d, 0, n * n * sizeof(double));
	for(int i = 0; i < n; i++) {
		d[i][i] = 1 / sqrt(er[i]);
	}

	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0, *mat,
	    n, *d, n, 0, *s, n);
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans, n, n, n, 1.0, *s, n,
	    *mat, n, 0, *d, n);

	molecule->dipole[0] = 0;
	molecule->dipole[1] = 0;
	molecule->dipole[2] = 0;

	for(int a = 0; a < n; a++) {
		for(int b = 0; b < n; b++) {
			molecule->dipole[0] += 2 * molecule->density[a][b]
			    * molecule->mux[a][b];
			molecule->dipole[1] += 2 * molecule->density[a][b]
			    * molecule->muy[a][b];
			molecule->dipole[2] += 2 * molecule->density[a][b]
			    * molecule->muz[a][b];
		}
	}

	for(int i = 0; i < molecule->num_atoms; i++) {
		molecule->dipole[0] += molecule->atoms[i].z * molecule->atoms[i].pos[0];
		molecule->dipole[1] += molecule->atoms[i].z * molecule->atoms[i].pos[1];
		molecule->dipole[2] += molecule->atoms[i].z * molecule->atoms[i].pos[2];
	}

	free(*mat);
	free(*s);
	free(*d);
	free(er);
	free(ei);
	free(mat);
	free(s);
	free(d);
}

