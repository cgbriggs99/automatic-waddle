/*
 * output.h
 *
 *  Created on: Sep 12, 2017
 *      Author: connor
 */

#ifndef OUTPUT_H_
#define OUTPUT_H_

extern void printDists(const molecule_t *molecule);

extern void printAngles(const molecule_t *molecule);

extern void printRotor(const molecule_t *molecule);

extern void printMoments(const molecule_t *molecule);

extern void printEigenvalues(const molecule_t *molecule);

extern void printFreqs(const molecule_t *molecule);

extern void outputSFC(molecule_t *molecule);

extern void printArray(char *title, double **data, int r, int c);

extern void printMP2(const molecule_t *molecule);

#endif /* OUTPUT_H_ */
