/*
 * calculations2.c
 *
 *  Created on: Nov 1, 2017
 *      Author: cgbri
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../calculations.h"
#include "../input.h"
#include "../output.h"
#include "../molecule.h"

#define EPS 0.0000000001

static int comparedd(const void *a, const void *b) {
	if(**((double **) a) > **((double **) b)) {
		return (1);
	} else {
		return (-1);
	}
}

static int compared(const void *a, const void *b) {
	if(*((double *) a) > *((double *) b)) {
		return (1);
	} else {
		return (-1);
	}
}

void calculateMP2(molecule_t *molecule) {
	int orb = molecule->orbitals;

	memset(molecule->mo_two_electron, 0,
	    TEI(orb, orb, orb, orb) * sizeof(double));

	for(int p = 0, pqrs = 0; p < molecule->orbitals; p++) {
		for(int q = 0; q <= p; q++) {
			for(int r = 0; r <= p; r++) {
				for(int s = 0; s <= ((p == r)? q: r); s++, pqrs++) {
					molecule->mo_two_electron[pqrs] = 0;
					for(int mu = 0; mu < molecule->orbitals; mu++) {
						for(int nu = 0; nu < molecule->orbitals; nu++) {
							for(int lam = 0; lam < molecule->orbitals; lam++) {
								for(int sig = 0; sig < molecule->orbitals;
								        sig++) {
									molecule->mo_two_electron[pqrs] += molecule
									    ->molecular_orbitals[mu][p]
									    * molecule->molecular_orbitals[nu][q]
									    * molecule->molecular_orbitals[lam][r]
									    * molecule->molecular_orbitals[sig][s]
									    * molecule->two_electron[TEI(mu, nu,
									        lam, sig)];
								}
							}
						}
					}
				}
			}
		}
	}

	molecule->mp2_energy = 0;

	for(int i = 0; i < molecule->electrons / 2; i++) {
		for(int j = 0; j < molecule->electrons / 2; j++) {
			for(int a = molecule->electrons / 2; a < molecule->orbitals; a++) {
				for(int b = molecule->electrons / 2; b < molecule->orbitals;
				        b++) {
					molecule->mp2_energy += molecule->mo_two_electron[TEI(i, a,
					    j, b)]
					    * (2 * molecule->mo_two_electron[TEI(i, a, j, b)]
					        - molecule->mo_two_electron[TEI(i, b, j, a)])
					    / (molecule->molecular_eigs[i][i]
					        + molecule->molecular_eigs[j][j]
					        - molecule->molecular_eigs[a][a]
					        - molecule->molecular_eigs[b][b]);
				}
			}
		}
	}

	molecule->tot_mp2 = molecule->mp2_energy + molecule->scf_energy;
}

/*
 * Calculate the spin-orbital two electron integrals.
 */
void calculateSOTEI(molecule_t *molecule) {
	int orbs = molecule->orbitals, elec = molecule->electrons / 2;
	for(int i = 0; i < 2 * orbs; i++) {
		for(int j = 0; j < 2 * orbs; j++) {
			for(int k = 0; k < 2 * orbs; k++) {
				for(int l = 0; l < 2 * orbs; l++) {
					molecule->spin_two_electron[i][j][k][l] = molecule
					    ->mo_two_electron[TEI(i / 2, k / 2, j / 2, l / 2)]
					    * ((i % 2) == (k % 2)) * ((j % 2) == (l % 2))
					    - molecule->mo_two_electron[TEI(i / 2, l / 2, j / 2,
					        k / 2)] * ((i % 2) == (l % 2))
					        * ((j % 2) == (k % 2));
				}
			}
		}
	}
}

/*
 * Find the intermediates for the CCSD.
 */
void calculateIntermediates(molecule_t *molecule) {
	int elecs = molecule->electrons, orbs = molecule->orbitals * 2;
	double x;
#define OCC_LOW 0
#define OCC_HIGH elecs
#define UNOCC_LOW elecs
#define UNOCC_HIGH orbs
#define MN_LOW 0
#define MN_HIGH elecs

	for(int i = OCC_LOW; i < OCC_HIGH; i++) {
		for(int j = OCC_LOW; j < OCC_HIGH; j++) {
			for(int a = UNOCC_LOW; a < UNOCC_HIGH; a++) {
				for(int b = UNOCC_LOW; b < UNOCC_HIGH; b++) {
					molecule->tpe2[i][j][a][b] =
					        molecule->t2_amplitude[i][j][a][b]
					            + (molecule->t1_amplitude[i][a]
					                * molecule->t1_amplitude[j][b]
					                - molecule->t1_amplitude[i][b]
					                    * molecule->t1_amplitude[j][a]) / 2.0;
					molecule->tpe1[i][j][a][b] =
					        molecule->t2_amplitude[i][j][a][b]
					            + (molecule->t1_amplitude[i][a]
					                * molecule->t1_amplitude[j][b])
					            - (molecule->t1_amplitude[i][b]
					                * molecule->t1_amplitude[j][a]);
				}
			}
		}
	}

	//Start F
	for(int a = UNOCC_LOW; a < UNOCC_HIGH; a++) {
		for(int e = UNOCC_LOW; e < UNOCC_HIGH; e++) {
			molecule->inter_f[a][e] = (a != e)? molecule->spin_fock[a][e]: 0;

			for(int m = MN_LOW; m < MN_HIGH; m++) {
				molecule->inter_f[a][e] -= (molecule->spin_fock[m][e]
				    * molecule->t1_amplitude[m][a]) / 2.0;
			}

			for(int m = MN_LOW; m < MN_HIGH; m++) {
				for(int f = OCC_LOW; f < OCC_HIGH; f++) {
					molecule->inter_f[a][e] += molecule->t1_amplitude[m][f]
					    * molecule->spin_two_electron[m][a][f][e];
				}
			}

			for(int m = MN_LOW; m < MN_HIGH; m++) {
				for(int f = OCC_LOW; f < OCC_HIGH; f++) {
					for(int n = MN_LOW; n < MN_HIGH; n++) {
						molecule->inter_f[a][e] -= (molecule->tpe2[m][n][a][f]
						    * molecule->spin_two_electron[m][n][e][f]) / 2.0;
					}
				}
			}
		}
	}

	for(int m = MN_LOW; m < MN_HIGH; m++) {
		for(int i = OCC_LOW; i < OCC_HIGH; i++) {
			molecule->inter_f[m][i] = (m != i)? molecule->spin_fock[m][i]: 0;

			for(int e = UNOCC_LOW; e < UNOCC_HIGH; e++) {
				molecule->inter_f[m][i] += (molecule->t1_amplitude[i][e]
				    * molecule->spin_fock[m][e]) / 2.0;
			}

			for(int e = UNOCC_LOW; e < UNOCC_HIGH; e++) {
				for(int n = MN_LOW; n < MN_HIGH; n++) {
					molecule->inter_f[m][i] += molecule->t1_amplitude[n][e]
					    * molecule->spin_two_electron[m][n][i][e];
				}
			}

			for(int e = UNOCC_LOW; e < UNOCC_HIGH; e++) {
				for(int n = MN_LOW; n < MN_HIGH; n++) {
					for(int f = UNOCC_LOW; f < UNOCC_HIGH; f++) {
						molecule->inter_f[m][i] += (molecule->tpe2[i][n][e][f]
						    * molecule->spin_two_electron[m][n][e][f]) / 2.0;
					}
				}
			}
		}
	}

	for(int m = MN_LOW; m < MN_HIGH; m++) {
		for(int e = UNOCC_LOW; e < UNOCC_HIGH; e++) {
			molecule->inter_f[m][e] = molecule->spin_fock[m][e];

			for(int n = MN_LOW; n < MN_HIGH; n++) {
				for(int f = UNOCC_LOW; f < UNOCC_HIGH; f++) {
					molecule->inter_f[m][e] += molecule->t1_amplitude[n][f]
					    * molecule->spin_two_electron[m][n][e][f];
				}
			}
		}
	}

	//Start W
	for(int m = MN_LOW; m < MN_HIGH; m++) {
		for(int n = MN_LOW; n < MN_HIGH; n++) {
			for(int i = OCC_LOW; i < OCC_HIGH; i++) {
				for(int j = OCC_LOW; j < OCC_HIGH; j++) {
					molecule->inter_w[m][n][i][j] =
					        molecule->spin_two_electron[m][n][i][j];

					for(int e = UNOCC_LOW; e < UNOCC_HIGH; e++) {
						molecule->inter_w[m][n][i][j] +=
						        molecule->t1_amplitude[j][e]
						            * molecule->spin_two_electron[m][n][i][e];
						molecule->inter_w[m][n][i][j] -=
						        molecule->t1_amplitude[i][e]
						            * molecule->spin_two_electron[m][n][j][e];
					}

					for(int e = UNOCC_LOW; e < UNOCC_HIGH; e++) {
						for(int f = UNOCC_LOW; f < UNOCC_HIGH; f++) {
							molecule->inter_w[m][n][i][j] +=
							        (molecule->tpe1[i][j][e][f]
							            * molecule->spin_two_electron[m][n][e][f])
							            / 4.0;
						}
					}
				}
			}
		}
	}

	for(int a = UNOCC_LOW; a < UNOCC_HIGH; a++) {
		for(int b = UNOCC_LOW; b < UNOCC_HIGH; b++) {
			for(int e = UNOCC_LOW; e < UNOCC_HIGH; e++) {
				for(int f = UNOCC_LOW; f < UNOCC_HIGH; f++) {
					molecule->inter_w[a][b][e][f] =
					        molecule->spin_two_electron[a][b][e][f];

					for(int m = MN_LOW; m < MN_HIGH; m++) {
						molecule->inter_w[a][b][e][f] -=
						        molecule->t1_amplitude[m][b]
						            * molecule->spin_two_electron[a][m][e][f];
						molecule->inter_w[a][b][e][f] +=
						        molecule->t1_amplitude[m][a]
						            * molecule->spin_two_electron[b][m][e][f];
					}

					for(int m = MN_LOW; m < MN_HIGH; m++) {
						for(int n = MN_LOW; n < MN_HIGH; n++) {
							molecule->inter_w[a][b][e][f] +=
							        (molecule->tpe1[m][n][a][b]
							            * molecule->spin_two_electron[m][n][e][f])
							            / 4.0;
						}
					}
				}
			}
		}
	}

	for(int m = MN_LOW; m < MN_HIGH; m++) {
		for(int b = UNOCC_LOW; b < UNOCC_HIGH; b++) {
			for(int e = UNOCC_LOW; e < UNOCC_HIGH; e++) {
				for(int j = OCC_LOW; j < OCC_HIGH; j++) {
					molecule->inter_w[m][b][e][j] =
					        molecule->spin_two_electron[m][b][e][j];

					for(int f = UNOCC_LOW; f < UNOCC_HIGH; f++) {
						molecule->inter_w[m][b][e][j] +=
						        molecule->t1_amplitude[j][f]
						            * molecule->spin_two_electron[m][b][e][f];
					}

					for(int n = MN_LOW; n < MN_HIGH; n++) {
						molecule->inter_w[m][b][e][j] -=
						        molecule->t1_amplitude[n][b]
						            * molecule->spin_two_electron[m][n][e][j];
					}

					for(int n = MN_LOW; n < MN_HIGH; n++) {
						for(int f = UNOCC_LOW; f < UNOCC_HIGH; f++) {
							molecule->inter_w[m][b][e][j] -= (molecule
							    ->t2_amplitude[j][n][f][b] / 2.0
							    + molecule->t1_amplitude[j][f]
							        * molecule->t1_amplitude[n][b])
							    * molecule->spin_two_electron[m][n][e][f];
						}
					}
				}
			}
		}
	}
}

/*
 * Finds the amplitudes.
 */
static void calculateT1(molecule_t *molecule, double **t1);
static void calculateT2(molecule_t *molecule, double ****t2);

/*
 * Find the CC energy.
 */
void calculateCCEnergy(molecule_t *molecule) {
	int orbs = 2 * molecule->orbitals, elecs = molecule->electrons;
	double **t1, ****t2, **h, **f, **er, **ei, **ev, **work, **sort;

	t1 = calloc(orbs, sizeof(double *));
	sort = calloc(orbs, sizeof(double*));
	t2 = calloc(orbs, sizeof(double ***));
	h = calloc(orbs, sizeof(double *));
	f = calloc_contiguous(2, sizeof(double), orbs, orbs);
	er = calloc_contiguous(2, sizeof(double), orbs, orbs);
	ei = calloc_contiguous(2, sizeof(double), orbs, orbs);
	ev = calloc_contiguous(2, sizeof(double), orbs, orbs);
	work = calloc_contiguous(2, sizeof(double), orbs, orbs);
	for(int i = 0; i < orbs; i++) {
		t1[i] = calloc(orbs, sizeof(double));
		t2[i] = calloc(orbs, sizeof(double **));
		h[i] = calloc(orbs, sizeof(double **));
		for(int j = 0; j < orbs; j++) {
			t2[i][j] = calloc(orbs, sizeof(double *));
			for(int k = 0; k < orbs; k++) {
				t2[i][j][k] = calloc(orbs, sizeof(double));
			}
		}
	}

	for(int i = 0; i < orbs; i++) {
		for(int j = 0; j < orbs; j++) {
			h[i][j] = 0;
			for(int k = 0; k < orbs / 2; k++) {
				for(int l = 0; l < orbs / 2; l++) {
					if(i % 2 == j % 2) {
						h[i][j] += molecule->molecular_orbitals[k][i / 2]
						    * molecule->molecular_orbitals[l][j / 2]
						    * molecule->hamiltonian[k][l];
					}
				}
			}
		}
	}
	printArray("\nHamiltonian\n", h, orbs, orbs);

	/*
	 * Find spin orbitals.
	 */
	calculateSOTEI(molecule);

	/*
	 * Find spin Fock matrix.
	 */
	for(int i = 0; i < orbs; i++) {
		for(int j = 0; j < orbs; j++) {
			molecule->spin_fock[i][j] = h[i][j];
			for(int k = 0; k < molecule->electrons; k++) {
				molecule->spin_fock[i][j] +=
				        molecule->spin_two_electron[i][k][j][k];
			}
		}
	}

	for(int i = 0; i < orbs; i++) {
		for(int j = 0; j < orbs; j++) {
			f[i][j] = molecule->spin_fock[i][j];
		}
	}

	printArray("\nFock\n", f, orbs, orbs);

	/*
	 * Find initial cluster amplitudes.
	 */
	for(int i = 0; i < molecule->orbitals; i++) {
		for(int j = 0; j < molecule->orbitals; j++) {
			t1[i][j] = 0;
			molecule->t1_amplitude[i][j] = 0;
		}
	}

	for(int i = 0; i < elecs; i++) {
		for(int j = 0; j < elecs; j++) {
			for(int k = elecs; k < orbs; k++) {
				for(int l = elecs; l < orbs; l++) {
					t2[i][j][k][l] = molecule->spin_two_electron[i][j][k][l]
					    / (molecule->spin_fock[i][i] + molecule->spin_fock[j][j]
					        - molecule->spin_fock[k][k]
					        - molecule->spin_fock[l][l]);
					molecule->t2_amplitude[i][j][k][l] = molecule
					    ->spin_two_electron[i][j][k][l]
					    / (molecule->spin_fock[i][i] + molecule->spin_fock[j][j]
					        - molecule->spin_fock[k][k]
					        - molecule->spin_fock[l][l]);
				}
			}
		}
	}

	double energy = 0, elast;

	for(int i = 0; i < elecs; i++) {
		for(int j = 0; j < elecs; j++) {
			for(int a = elecs; a < orbs; a++) {
				for(int b = elecs; b < orbs; b++) {
					energy += molecule->spin_two_electron[i][j][a][b]
					    * molecule->t2_amplitude[i][j][a][b] / 4;
				}
			}
		}
	}
	printf("\nTest energy\t%.17f", energy);

	printf("\nEcc\n");

	do {
		elast = energy;
		/*
		 * Find intermediates.
		 */
		calculateIntermediates(molecule);

//	for(int i = 0; i < orbs; i++) {
//		for(int a = 0; a < orbs; a++) {
//			if(fabs(molecule->inter_f[i][a]) > 1) {
//				printf("\nF:\t%f\trc:\t%d\t%d\n", molecule->inter_f[i][a], i,
//				    a);
//			}
//		}
//	}

//	for(int i = 0; i < orbs; i++) {
//		for(int a = 0; a < orbs; a++) {
//			for(int b = 0; b < orbs; b++) {
//				for(int c = 0; c < orbs; c++) {
//					if(fabs(molecule->inter_w[i][a][b][c]) > 1) {
//						printf("\nW:\t%f\tpos:\t%d\t%d\t%d\t%d\n",
//						    molecule->inter_w[i][a][b][c], i, a, b, c);
//					}
//
//				}
//			}
//		}
//	}

		/*
		 * Update cluster amplitudes.
		 */
		calculateT1(molecule, t1);

		calculateT2(molecule, t2);

		/*
		 * Test convergence.
		 */
		energy = 0;
		for(int i = 0; i < elecs; i++) {
			for(int a = elecs; a < orbs; a++) {
				energy += molecule->spin_fock[i][a] * t1[i][a];
			}
		}

		for(int i = 0; i < elecs; i++) {
			for(int j = 0; j < elecs; j++) {
				for(int a = elecs; a < orbs; a++) {
					for(int b = elecs; b < orbs; b++) {
						energy += t2[i][j][a][b]
						    * molecule->spin_two_electron[i][j][a][b] / 4;
					}
				}
			}
		}

		for(int i = 0; i < elecs; i++) {
			for(int j = 0; j < elecs; j++) {
				for(int a = elecs; a < orbs; a++) {
					for(int b = elecs; b < orbs; b++) {
						energy += molecule->spin_two_electron[i][j][a][b]
						    * t1[i][a] * t1[j][b] / 2;
					}
				}
			}
		}

		for(int i = 0; i < orbs; i++) {
			for(int j = 0; j < orbs; j++) {
				molecule->t1_amplitude[i][j] = t1[i][j];
				for(int k = 0; k < orbs; k++) {
					for(int l = 0; l < orbs; l++) {
						molecule->t2_amplitude[i][j][k][l] = t2[i][j][k][l];
						t2[i][j][k][l] = 0;
					}
				}
				t1[i][j] = 0;
			}
		}

		printf("%.17f\n", energy);
	} while(fabs(energy - elast) > EPS);

	molecule->ccsd_energy = energy;

	for(int i = 0; i < orbs; i++) {
		free(t1[i]);
		for(int j = 0; j < orbs; j++) {
			for(int k = 0; k < orbs; k++) {
				free(t2[i][j][k]);
			}
			free(t2[i][j]);
		}
		free(t2[i]);
	}
	free(t1);
	free(t2);
	free_mult_contig(5, f, ev, er, ei, work);
}

static void calculateT1(molecule_t *molecule, double **t1) {
	int elecs = molecule->electrons, orbs = molecule->orbitals * 2;
	for(int i = OCC_LOW; i < OCC_HIGH; i++) {
		for(int a = UNOCC_LOW; a < UNOCC_HIGH; a++) {
			t1[i][a] = 0;
			double sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0, sum5 = 0, sum6 = 0;
			for(int e = UNOCC_LOW; e < UNOCC_HIGH; e++) {
				sum1 += molecule->t1_amplitude[i][e] * molecule->inter_f[a][e];
			}
			for(int m = MN_LOW; m < MN_HIGH; m++) {
				sum2 += molecule->t1_amplitude[m][a] * molecule->inter_f[m][i];
			}
			for(int m = MN_LOW; m < MN_HIGH; m++) {
				for(int e = UNOCC_LOW; e < UNOCC_HIGH; e++) {
					sum3 += molecule->t2_amplitude[i][m][a][e]
					    * molecule->inter_f[m][e];
				}
			}
			for(int n = MN_LOW; n < MN_HIGH; n++) {
				for(int f = UNOCC_LOW; f < UNOCC_HIGH; f++) {
					sum4 += molecule->t1_amplitude[n][f]
					    * molecule->spin_two_electron[n][a][i][f];
				}
			}
			for(int m = MN_LOW; m < MN_HIGH; m++) {
				for(int e = UNOCC_LOW; e < UNOCC_HIGH; e++) {
					for(int f = UNOCC_LOW; f < UNOCC_HIGH; f++) {
						sum5 += molecule->t2_amplitude[i][m][e][f]
						    * molecule->spin_two_electron[m][a][e][f];
					}
				}
			}
			for(int m = MN_LOW; m < MN_HIGH; m++) {
				for(int e = UNOCC_LOW; e < UNOCC_HIGH; e++) {
					for(int n = MN_LOW; n < MN_HIGH; n++) {
						sum6 += molecule->t2_amplitude[m][n][a][e]
						    * molecule->spin_two_electron[n][m][e][i];
					}
				}
			}
			t1[i][a] = (molecule->spin_fock[i][a] + sum1 - sum2 + sum3 - sum4
			    - sum5 / 2 - sum6 / 2)
			    / (molecule->spin_fock[i][i] - molecule->spin_fock[a][a]);

			if(fabs(t1[i][a]) > 0.5) {
				printf("\nnum and den t1\t%f\t\t%f\n",
				    (molecule->spin_fock[i][i] - molecule->spin_fock[a][a]),
				    t1[i][a]);
			}
		}
	}
}

static void calculateT2(molecule_t *molecule, double ****t2) {
	int orbs = molecule->orbitals * 2, elecs = molecule->electrons;

	for(int i = OCC_LOW; i < OCC_HIGH; i++) {
		for(int j = OCC_LOW; j < OCC_HIGH; j++) {
			for(int a = UNOCC_LOW; a < UNOCC_HIGH; a++) {
				for(int b = UNOCC_LOW; b < UNOCC_HIGH; b++) {
					double sum[14];

					for(int x = 0; x < 14; x++) {
						sum[x] = 0;
					}

					t2[i][j][a][b] = 0;

					for(int e = UNOCC_LOW; e < UNOCC_HIGH; e++) {
						double s = 0;
						for(int m = MN_LOW; m < MN_HIGH; m++) {
							s += molecule->t1_amplitude[m][b]
							    * molecule->inter_f[m][e];
						}
						sum[0] += molecule->t2_amplitude[i][j][a][e]
						    * (molecule->inter_f[b][e] - s / 2);
					}

					for(int e = UNOCC_LOW; e < UNOCC_HIGH; e++) {
						double s = 0;
						for(int m = MN_LOW; m < MN_HIGH; m++) {
							s += molecule->t1_amplitude[m][a]
							    * molecule->inter_f[m][e];
						}
						sum[1] += molecule->t2_amplitude[i][j][b][e]
						    * (molecule->inter_f[a][e] - s / 2);
					}

					for(int m = MN_LOW; m < MN_HIGH; m++) {
						double s = 0;
						for(int e = UNOCC_LOW; e < UNOCC_HIGH; e++) {
							s += molecule->t1_amplitude[j][e]
							    * molecule->inter_f[m][e];
						}
						sum[2] += molecule->t2_amplitude[i][m][a][b]
						    * (molecule->inter_f[m][j] + s / 2);
					}

					for(int m = MN_LOW; m < MN_HIGH; m++) {
						double s = 0;
						for(int e = UNOCC_LOW; e < UNOCC_HIGH; e++) {
							s += molecule->t1_amplitude[i][e]
							    * molecule->inter_f[m][e];
						}
						sum[3] += molecule->t2_amplitude[j][m][a][b]
						    * (molecule->inter_f[m][i] + s / 2);
					}

					for(int m = MN_LOW; m < MN_HIGH; m++) {
						for(int n = MN_LOW; n < MN_HIGH; n++) {
							sum[4] += molecule->tpe1[m][n][a][b]
							    * molecule->inter_w[m][n][i][j] / 2;
						}
					}

					for(int e = UNOCC_LOW; e < UNOCC_HIGH; e++) {
						for(int f = UNOCC_LOW; f < UNOCC_HIGH; f++) {
							sum[5] += molecule->tpe1[i][j][e][f]
							    * molecule->inter_w[a][b][e][f] / 2;
						}
					}

					for(int m = MN_LOW; m < MN_HIGH; m++) {
						for(int e = UNOCC_LOW; e < UNOCC_HIGH; e++) {
							sum[6] += molecule->t2_amplitude[i][m][a][e]
							    * molecule->inter_w[m][b][e][j]
							    - molecule->t1_amplitude[i][a]
							        * molecule->t1_amplitude[m][e]
							        * molecule->spin_two_electron[m][b][e][j];
						}
					}

					for(int m = MN_LOW; m < MN_HIGH; m++) {
						for(int e = UNOCC_LOW; e < UNOCC_HIGH; e++) {
							sum[7] += molecule->t2_amplitude[j][m][a][e]
							    * molecule->inter_w[m][b][e][i]
							    - molecule->t1_amplitude[j][a]
							        * molecule->t1_amplitude[m][e]
							        * molecule->spin_two_electron[m][b][e][i];
						}
					}

					for(int m = MN_LOW; m < MN_HIGH; m++) {
						for(int e = UNOCC_LOW; e < UNOCC_HIGH; e++) {
							sum[8] += molecule->t2_amplitude[i][m][b][e]
							    * molecule->inter_w[m][a][e][j]
							    - molecule->t1_amplitude[i][b]
							        * molecule->t1_amplitude[m][e]
							        * molecule->spin_two_electron[m][a][e][j];
						}
					}

					for(int m = MN_LOW; m < MN_HIGH; m++) {
						for(int e = UNOCC_LOW; e < UNOCC_HIGH; e++) {
							sum[9] += molecule->t2_amplitude[j][m][b][e]
							    * molecule->inter_w[m][a][e][i]
							    - molecule->t1_amplitude[j][b]
							        * molecule->t1_amplitude[m][e]
							        * molecule->spin_two_electron[m][a][e][i];
						}
					}

					for(int e = UNOCC_LOW; e < UNOCC_HIGH; e++) {
						sum[10] += molecule->t1_amplitude[i][e]
						    * molecule->spin_two_electron[a][b][e][j];
					}

					for(int e = UNOCC_LOW; e < UNOCC_HIGH; e++) {
						sum[11] += molecule->t1_amplitude[j][e]
						    * molecule->spin_two_electron[a][b][e][i];
					}

					for(int m = MN_LOW; m < MN_HIGH; m++) {
						sum[12] += molecule->t1_amplitude[m][a]
						    * molecule->spin_two_electron[m][b][i][j];
					}

					for(int m = MN_LOW; m < MN_HIGH; m++) {
						sum[13] += molecule->t1_amplitude[m][b]
						    * molecule->spin_two_electron[m][a][i][j];
					}

					t2[i][j][a][b] = (molecule->spin_two_electron[i][j][a][b]
					    + sum[0] - sum[1] - sum[2] + sum[3] + sum[4] + sum[5]
					    + sum[6] - sum[7] - sum[8] + sum[9] + sum[10] - sum[11]
					    - sum[12] + sum[13])
					    / (molecule->spin_fock[i][i] + molecule->spin_fock[j][j]
					        - molecule->spin_fock[a][a]
					        - molecule->spin_fock[b][b]);
					if(fabs(t2[i][j][a][b]) > 0.5) {
						printf("\nnum and den t2\t%f\t\t%f\n",
						    (molecule->spin_fock[i][i]
						        + molecule->spin_fock[j][j]
						        - molecule->spin_fock[a][a]
						        - molecule->spin_fock[b][b]), t2[i][j][a][b]);
					}
				}
			}
		}
	}
}
