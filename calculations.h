/*
 * calculations.h
 *
 *  Created on: Sep 5, 2017
 *      Author: connor
 */

#ifndef CALCULATIONS_H_
#define CALCULATIONS_H_

#include "./molecule.h"

//Defines constants that will be necessary for calculations.
#define PLANCK 3.9905e31 //amu angstrom^2 / s
#define C 2997924580000000000	//angstrom / s

/*
 * Finds all of the distances between atom pairs in the molecule.
 */
extern void distances(molecule_t *molecule);

/*
 * Finds all of the angles between atom triples in the molecule in radians.
 */
extern void angles(molecule_t *molecule);

/*
 * Finds all of the out-of-plane angles of a molecule.
 */
extern void planeAngles(molecule_t *molecule);

/*
 * Finds all of the torsion angles of a molecule.
 */
extern void torsionAngles(molecule_t *molecule);

/*
 * Finds the center of mass of the molecule..
 */
extern void centerOfMass(molecule_t *molecule);

/*
 * Finds the moments of inertia of the molecule.
 */
extern void inertialMoments(molecule_t *molecule);

/*
 * Finds the principle moments of inertia.
 */
extern void principalMoments(molecule_t *molecule);

/*
 * Finds the rotational constants of the molecule.
 */
extern void rotationalConstants(molecule_t *molecule);

/*
 * Centers a molecule around its center of mass.
 */
extern void center(molecule_t *molecule);

/*
 * Finds the kind of rotor the molecule is.
 */
extern void rotor(molecule_t *molecule);

/*
 * Mass-weight the Hessian matrix.
 */
extern void weightHessian(molecule_t *molecule);

/*
 * Find the eigenvalues of the mass-weighted Hessian matrix.
 */
extern void eigensHessian(molecule_t *molecule);

/*
 * Calculate the core hamiltonian.
 */
extern void hamiltonian(molecule_t *molecule);

/*
 * Find the inverse square root of the molecule's overlap matrix.
 */
extern void sqrtMolecule(molecule_t *molecule);

/*
 * Calculate the self-consistent fock matrix.
 */
extern void findFock(molecule_t *molecule);
extern void findFockPrint(molecule_t *molecule);

/*
 * Find the density and energy.
 */
extern void calculateDensEnergy(molecule_t *molecule);

/*
 * Find the charges and dipole moment.
 */
extern void calculateCharges(molecule_t *molecule);

/*
 * Find the MP2 energy.
 */
extern void calculateMP2(molecule_t *molecule);

/*
 * Calculate the spin-orbital two electron integrals.
 */
extern void calculateSOTEI(molecule_t *molecule);

/*
 * Find the intermediates for the CCSD.
 */
extern void calculateIntermediates(molecule_t *molecule);

/*
 * Find the CC energy.
 */
extern void calculateCCEnergy(molecule_t *molecule);

/*
 * Find the CCSDT energy
 */
extern void calculateCCSDTEnergy(molecule_t *molecule);

/*
 * Use DIIS to help SCF
 */
extern void calculateSCFDIIS(molecule_t *molecule);


#endif /* CALCULATIONS_H_ */
