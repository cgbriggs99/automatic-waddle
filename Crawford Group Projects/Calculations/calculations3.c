/*
 * calculations3.c
 *
 *  Created on: Nov 8, 2017
 *      Author: cgbri
 */

#include "../lapack_local.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include "../calculations.h"
#include "../input.h"
#include "../output.h"
#include "../molecule.h"
#include "../Memory/mem_custom.h"

static int comparedd(const void *a, const void *b) {
	if(**((double **) a) > **((double **) b)) {
		return (1);
	} else {
		return (-1);
	}
}

/*
 * Use DIIS to help SCF
 */
void calculateSCFDIIS(molecule_t *molecule) {
#define EPS 0.0000000000001
#define DEL 0.0000000000001
	double **fs[6], **es[6], **b, *c;
	int **piv;

	hamiltonian(molecule);
	sqrtMolecule(molecule);

	int n = molecule->orbitals;	//So that the same thing does not need to be typed repeatedly.
	int count = 0;

	double elec, energy = 0, elast, rms;

	double **f0, **f1, **f2, **c0, **c1, **d0, **d1, **work1, **work2, **work3,
	        **ham, **shalf, **s;

	double **sort;

	f0 = calloc_contiguous(2, sizeof(double), n, n);
	f1 = calloc_contiguous(2, sizeof(double), n, n);
	f2 = calloc_contiguous(2, sizeof(double), n, n);
	c0 = calloc_contiguous(2, sizeof(double), n, n);
	c1 = calloc_contiguous(2, sizeof(double), n, n);
	d0 = calloc_contiguous(2, sizeof(double), n, n);
	d1 = calloc_contiguous(2, sizeof(double), n, n);
	work1 = calloc_contiguous(2, sizeof(double), n, n);
	work2 = calloc_contiguous(2, sizeof(double), n, n);
	work3 = calloc_contiguous(2, sizeof(double), n, n);
	ham = calloc_contiguous(2, sizeof(double), n, n);
	shalf = calloc_contiguous(2, sizeof(double), n, n);
	sort = calloc_contiguous(2, sizeof(double), n, n);
	b = calloc_contiguous(2, sizeof(double), 7, 7);
	c = calloc(7, sizeof(double));
	s = calloc_contiguous(2, sizeof(double), n, n);
	piv = calloc_contiguous(2, sizeof(double), 7, 7);
	for(int i = 0; i < 6; i++) {
		fs[i] = calloc_contiguous(2, sizeof(double), n, n);
		es[i] = calloc_contiguous(2, sizeof(double), n, n);
	}

	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			s[i][j] = molecule->overlap[i][j];
			shalf[i][j] = molecule->symmetric[i][j];
		}
	}
	printf("\nElec\t\tEnergy\t\tDiff\t\tRMS\n");
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

		//DIIS extrapolation.
		memcpy(*(fs[count % 6]), *f0, n * n * sizeof(double));
		cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0, *s,
		    n, *d0, n, 0, *work1, n);
		cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0,
		    *work1, n, *f0, n, 0, *work2, n);
		cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0,
		    *f0, n, *d0, n, 0, *work1, n);
		cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0,
		    *work1, n, *s, n, 0, *work3, n);
		for(int i = 0; i < n; i++) {
			for(int j = 0; j < n; j++) {
				es[count % 6][i][j] = work3[i][j] - work2[i][j];
			}
		}
		if(count >= 6) {
			for(int i = 0; i < ((count > 6)? 6: count); i++) {
				for(int j = 0; j < ((count > 6)? 6: count); j++) {
					b[i][j] = 0;
					for(int k = 0; k < n; k++) {
						for(int l = 0; l < n; l++) {
							b[i][j] += es[i][k][l] * es[j][k][l];
						}
					}
				}
			}

			if(count < 6) {
				for(int i = 0; i < 6; i++) {
					for(int j = 0; j < 6; j++) {
						if(i < count && j < count) {
							continue;
						}
						if(i == j) {
							b[i][j] = 1;
						} else {
							b[i][j] = 0;
						}
					}
				}
			}

			for(int i = 0; i < 6; i++) {
				b[6][i] = -1;
				b[i][6] = -1;
				c[i] = 0;
			}
			b[6][6] = 0;
			c[6] = -1;
			LAPACKE_dgesv(LAPACK_ROW_MAJOR, 7, 1, *b, 7, *piv, c, 1);

			for(int i = 0; i < n; i++) {
				for(int j = 0; j < n; j++) {
					f2[i][j] = 0;
					for(int m = 0; m < 6; m++) {
						f2[i][j] += c[m] * fs[m][i][j];
					}
				}
			}

			cblas_dgemm(CblasRowMajor, CblasTrans, CblasNoTrans, n, n, n, 1.0,
			    *shalf, n, *f2, n, 0, *work1, n);
			cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0,
			    *work1, n, *shalf, n, 0, *f1, n);
		} else {
			cblas_dgemm(CblasRowMajor, CblasTrans, CblasNoTrans, n, n, n, 1.0,
			    *shalf, n, *f0, n, 0, *work1, n);
			cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0,
			    *work1, n, *shalf, n, 0, *f1, n);
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
		printf("%d\t%.15f\t%.15f\t%.15f\t%.15f\n", count, elec, energy, fabs(elast - energy), rms);

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

	free_mult_contig(16, c0, c1, d0, d1, f0, f1, f2, ham, shalf, work1, work2,
	    work3, sort, b, c, s);
	for(int i = 0; i < 6; i++) {
		free(fs[i]);
		free(es[i]);
	}
}
