/*
 * molecule_factory.cpp
 *
 *  Created on: Dec 4, 2018
 *      Author: cgbri
 */

#include "molecule_factory.hpp"

compchem::MoleculeFactory::MoleculeFactory() {
	;
}

compchem::Molecule *compchem::MoleculeFactory::buildMolecule(
        MoleculeFactoryStrategy &strat) {
	return (strat.build());
}

compchem::MoleculeFactory *compchem::MoleculeFactory::getSingleton() {
	if(singleton == nullptr) {
		singleton = new compchem::MoleculeFactory();
	}
	return (singleton);
}
