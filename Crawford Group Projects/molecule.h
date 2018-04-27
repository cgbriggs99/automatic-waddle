/*
 * molecule.h
 *
 *  Created on: Sep 10, 2017
 *      Author: connor
 */

#ifndef MOLECULE_H_
#define MOLECULE_H_

#ifdef __cplusplus
namespace compchem {
//extern "C" {
#endif

#define STRINGIFY(arg) #arg
#define STRINGIFY1(arg) STRINGIFY(arg)
#define PRINT printf("\nDebug in " __FILE__ " at " STRINGIFY1(__LINE__)\
		" successful\n"); fflush(stdout)

typedef struct {
	double pos[3];	// Position of the atom in space.
	int z;	// Atomic number.
	double mass;	// Atomic mass (amu).
	int charge;	// Charge (electron-charges).
	int id;	// ID for better searching.
	double true_charge;	//True charge of the atom.
} atom_t;

typedef struct molecule_t{
	atom_t *atoms;	// A list of atoms in the molecule.
	int num_atoms;	// The number of atoms in the molecule.
	double **distances;	// Distances between atom pairs.
	double ***angles;	/* Angles between atom triples. angles[i][j][k] is the angle between atom[j] and atom[k]
						 * with a base at atom[i].
						 */
	double ****plane_angles;	/* Out-of-plane angles. plane_angles[i][j][k][l] is the angle of atom[l] out of the
								 * plane created by atoms i, j, and k.
								 */
	double ****torsion_angles;	/* Torsion angles. torsion_angles[i][j][k][l] is the angular difference between
								 * pair [i, j] and pair[k, l].
								 */
	double inertial_moments[3][3];	// Moments of inertia.
	double principal_moments[3];	// Eigenvalues of inertial_moments, sorted from least to greatest.
	double rotational_constants[3];	// Constants relating to molecular rotations and vibrations.
	double mass;	// Total mass of the molecule in amu.
	double center_of_mass[3];	// Center of mass of the molecule.
	enum {
		SPHERICAL_TOP, LINEAR, SYMMETRIC, ASYMMETRIC
	} rotor;	// Kind of rotor.

	double **hessian;	//Hessian matrix for this molecule.
	int num_hessian;	//Dimension of the Hessian matrix.
	double *hessian_eigs;	//Eigenvalues of the Hessian matrix.

	double enuc;	//Nuclear repulsion energy.

	double **overlap;	//Atom-orbital overlap.
	double **kin_energy;	//Kinetic energies of electrons.
	double **attraction;	//Attraction energies of electrons.
	double **hamiltonian;	//Core hamiltonian.

	double *two_electron;

	double **symmetric;	//Symmetric orthogonalization matrix.
	double **symm_trans;	//Transpose of the symmetric orthogonalization matrix.
	double **symm_vecs;	//Eigenvectors of the Symmetric orthogonal matrix.
	double **fock;	//Fock matrix.
	double scf_energy;	//Energy of SCF.
	double **density;
	double **molecular_orbitals;	//Fock matrix eigenvectors.
	double **molecular_eigs;	//Eigenvalue matrix of the Fock matrix.
	double **mux, **muy, **muz;	//Components for the dipole moment.
	int orbitals;	//Number of orbitals.
	int electrons;	//Number of electrons.

	double dipole[3];	//Dipole moment.

	double *mo_two_electron;	//Two electron integral in MO basis.
	double mp2_energy;	//Moller-Plesset energy.
	double tot_mp2;	//Energy, including MP2 energy.

	double ****spin_two_electron;	//Spin orbitals.
	double ****t2_amplitude;	//T2 CCSD amplitude.
	double **t1_amplitude;	//T1 CCSD amplitude.
	double **spin_fock;	//Spin-orbital fock.
	double **inter_f;	//2-index intermediate.
	double ****inter_w;	//4-index intermediate.
	double ****tpe1, ****tpe2;	//Two particle excitation.

	double ccsd_energy;	//CCSD Energy.

	struct {
		int two_electron;
	} files;

} molecule_t;

#ifdef __cplusplus
extern "C" {
#endif
extern int TEI(int mu, int nu, int lam, int sig);

/*
 * Creates a new allocated molecule with the given atoms.
 */
extern molecule_t *newMolecule(const atom_t *atoms, int num);

/*
 * Frees the space taken by a molecule.
 */
extern void deleteMolecule(molecule_t *molecule);

/*
 * Returns the average atomic mass of element z.
 */
extern double amu(int z);

extern int valence(int z);

extern int orbitals(int z);

#ifdef __cplusplus
}
}
#endif
#endif /* MOLECULE_H_ */
