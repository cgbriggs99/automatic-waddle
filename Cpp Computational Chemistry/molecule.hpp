/*
 * molecule.hpp
 *
 *  Created on: Apr 25, 2018
 *      Author: Connor
 */

#ifndef MOLECULE_HPP_
#define MOLECULE_HPP_


#include <stdio.h>
#include <stdarg.h>
#include "compchem.hpp"

#include <vector>
#include <vector>
#include <vector>
#include <vector>
#include <vector>
#include <vector>
#include <vector>
#include <vector>
#include <vector>


namespace compchem {

class Molecule;

class Atom {
	friend class Molecule;
	friend void compchem::input(compchem::Molecule **, FILE *);
private:
	compchem::FortranArray<double> *pos;
	double mass;
	double true_charge;
	int num;
	int charge;
public:
	Atom() {
		this->mass = 0;
		this->true_charge = 0;
		this->num = 0;
		this->charge = 0;
		this->pos = new FortranArray<double>(Dimension(3));
	}
	Atom(int num, double x, double y, double z);

	~Atom();

	int getCharge() {
		return (charge);
	}

	double getMass() {
		return (mass);
	}

	int getNum() {
		return (num);
	}

	compchem::FortranArray<double> &getPos() {
		return (*pos);
	}

	double getTrueCharge() {
		return (true_charge);
	}

	void setTrueCharge(double trueCharge) {
		true_charge = trueCharge;
	}
};

class Molecule {

public:
	typedef enum {
		SPHERICAL_TOP, LINEAR, SYMMETRIC, ASYMMETRIC
	} rotor_t;
private:
	friend void inputHessian(Molecule **, FILE *);
	friend void inputSCF(Molecule **, FILE *, FILE *, FILE *, FILE *,
			FILE *, FILE *, FILE *, FILE *, FILE *);
	typedef int filedesc;
	filedesc blocks;
	char *memory;

	//Counts.
	int occupied;
	int electrons;
	int orbitals;

	double scf_eps;

	//Atom data.
	std::vector<Atom> &atoms;
	int numatoms;
	double total_mass;

	//Geometry data.
	compchem::FortranArray<double> *distances;
	compchem::FortranArray<double> *angles;
	compchem::FortranArray<double> *plane_angles;
	compchem::FortranArray<double> *torsion_angles;
	compchem::FortranArray<double> *center_of_mass;
	compchem::FortranArray<double> *inertial_moments;	//TODO needs a getter.
	compchem::FortranArray<double> *principle_moments;
	compchem::FortranArray<double> *rotational_constants;

	rotor_t rotor;

	//Harmonic data.
	compchem::FortranArray<double> *hessian;
	compchem::FortranArray<double> *hessian_eigs;
	compchem::FortranArray<double> *vibrations;

	//Energies.
	double enuc;
	double scf_energy;
	compchem::FortranArray<double> *molecular_energies;
	double mp2_correction;
	double mp2_energy;

	//Integrals.
	compchem::FortranArray<double> *overlap;
	compchem::FortranArray<double> *kinetic;
	compchem::FortranArray<double> *attraction;
	compchem::FortranArray<double> *hamiltonian;
	compchem::FortranArray<double> *two_electron;
	compchem::FortranArray<double> *mo_two_electron;
	compchem::FortranArray<double> *spin_two_electron;

	//Important matrices.
	compchem::FortranArray<double> *orthogonal;
	compchem::FortranArray<double> *orthogonal_t;
	compchem::FortranArray<double> *orthogonal_eigvs;
	compchem::FortranArray<double> *fock;
	compchem::FortranArray<double> *density;
	compchem::FortranArray<double> *molecular_orbitals;
	compchem::FortranArray<double> *mux, *muy, *muz;
	compchem::FortranArray<double> *dipole;
	compchem::FortranArray<double> *spin_fock;

	//CCSD intermediates;
	compchem::FortranArray<double> *t1_amplitudes;
	compchem::FortranArray<double> *t2_amplitudes;
	compchem::FortranArray<double> *inter_f;
	compchem::FortranArray<double> *inter_w;
	compchem::FortranArray<double> *tpe1;
	compchem::FortranArray<double> *tpe2;
public:
	/*
	 * Calculation methods.
	 */
	void computeDists();
	void computeAngles();
	void computePlaneAngles();
	void computeTorsionAngles();
	void translateCOM();
	void momentsOfInertia();
	void rotations();

	void harmonics();

	void computeSCF(bool print = false);

	void computeMOTEI();
	void computeMP2();

	void computeCCSD();

	void computeCCSDT();

	void computeSCF_DIIS();

	void computeSCF_Symm();

	void computeCCSD_DIIS();

	void computeExcited();

	void computeDLExcited();

	Molecule(std::vector<Atom> &atoms, int num, double scf_eps = 0.000000000001);
	~Molecule();

	compchem::FortranArray<double> &getAngles() {
		return (*angles);
	}

	std::vector<Atom> &getAtoms() {
		return (atoms);
	}

	compchem::FortranArray<double> &getAttraction() {
		return (*attraction);
	}

	filedesc getBlocks() {
		return (blocks);
	}

	compchem::FortranArray<double> &getDensity() {
		return (*density);
	}

	compchem::FortranArray<double> &getDipole() {
		return (*dipole);
	}

	compchem::FortranArray<double> &getDistances() {
		return (*distances);
	}

	int getElectrons() {
		return (electrons);
	}

	double getEnuc() {
		return (enuc);
	}

	compchem::FortranArray<double> &getFock() {
		return (*fock);
	}

	compchem::FortranArray<double> &getHamiltonian() {
		return (*hamiltonian);
	}

	compchem::FortranArray<double> &getHessian() {
		return (*hessian);
	}

	compchem::FortranArray<double> &getHessianEigs() {
		return (*hessian_eigs);
	}

	int getHessianSize() {
		return (this->hessian->getShape()[0]);
	}

	compchem::FortranArray<double> &getInterF() {
		return (*inter_f);
	}

	compchem::FortranArray<double> &getInterW() {
		return (*inter_w);
	}

	compchem::FortranArray<double> &getKinetic() {
		return (*kinetic);
	}

	char* getMemory() {
		return (memory);
	}

	compchem::FortranArray<double> &getMoTwoElectron() {
		return (*mo_two_electron);
	}

	compchem::FortranArray<double> &getMolecularEnergies() {
		return (*molecular_energies);
	}

	compchem::FortranArray<double> &getMolecularOrbitals() {
		return (*molecular_orbitals);
	}

	double getMp2Correction() {
		return (mp2_correction);
	}

	double getMp2Energy() {
		return (mp2_energy);
	}

	compchem::FortranArray<double> &getMux() {
		return (*mux);
	}

	compchem::FortranArray<double> &getMuy() {
		return (*muy);
	}

	compchem::FortranArray<double> &getMuz() {
		return (*muz);
	}

	int getNumatoms() {
		return (numatoms);
	}

	int getOccupied() {
		return (occupied);
	}

	int getOrbitals() {
		return (orbitals);
	}

	compchem::FortranArray<double> &getOrthogonal() {
		return (*orthogonal);
	}

	compchem::FortranArray<double> &getOrthogonalEigvs() {
		return (*orthogonal_eigvs);
	}

	compchem::FortranArray<double> &getOrthogonalT() {
		return (*orthogonal_t);
	}

	compchem::FortranArray<double> &getOverlap() {
		return (*overlap);
	}

	compchem::FortranArray<double> &getPlaneAngles() {
		return (*plane_angles);
	}

	compchem::FortranArray<double> &getPrincipleMoments() {
		return (*principle_moments);
	}

	compchem::FortranArray<double> &getRotationalConstants() {
		return (*rotational_constants);
	}

	rotor_t getRotor() {
		return (rotor);
	}

	double getScfEnergy() {
		return (scf_energy);
	}

	compchem::FortranArray<double> &getSpinFock() {
		return (*spin_fock);
	}

	compchem::FortranArray<double> &getSpinTwoElectron() {
		return (*spin_two_electron);
	}

	compchem::FortranArray<double> &getT1Amplitudes() {
		return (*t1_amplitudes);
	}

	compchem::FortranArray<double> &getT2Amplitudes() {
		return (*t2_amplitudes);
	}

	compchem::FortranArray<double> &getTorsionAngles() {
		return (*torsion_angles);
	}

	double getTotalMass() {
		return (total_mass);
	}

	compchem::FortranArray<double> &getTpe1() {
		return (*tpe1);
	}

	compchem::FortranArray<double> &getTpe2() {
		return (*tpe2);
	}

	compchem::FortranArray<double> &getTwoElectron() {
		return (*two_electron);
	}

	void setEnuc(double enuc) {
		this->enuc = enuc;
	}

	compchem::FortranArray<double> &getCenterOfMass() {
		return (*center_of_mass);
	}

	compchem::FortranArray<double> &getInertialMoments() {
		return (*inertial_moments);
	}
};

int TEI(int mu, int nu, int lam, int sig);

double amu(int z);

int valence(int z);

int orbitals(int z);

}

#endif /* MOLECULE_HPP_ */
