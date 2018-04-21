/*
 * test.c
 *
 *  Created on: Nov 8, 2017
 *      Author: cgbri
 */

#include <math.h>
#include "./mem_custom.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(void) {
	srand(clock());

	double mat[10][10];
	for(int i = 0; i < 10; i++) {
		for(int j = 0; j < 10; j++) {
			mat[i][j] = rand() / ((double) RAND_MAX);
		}
	}

	double **cont = calloc_contiguous(2, sizeof(double), 10, 10), **dyn =
	        calloc_check(10, sizeof(double *)), **temp = calloc_contiguous(2,
	    sizeof(double), 10, 10), **work = calloc_contiguous(2, sizeof(double),
	    10, 10);
	for(int i = 0; i < 10; i++) {
		dyn[i] = calloc_check(10, sizeof(double));
	}

	for(int i = 0; i < 10; i++) {
		for(int j = 0; j < 10; j++) {
			temp[i][j] = cont[i][j] = mat[i][j];
			dyn[i][j] = mat[i][j];
		}
	}

	for(int i = 0; i < 10; i++) {
		for(int j = 0; j < 10; j++) {
			dyn[i][j] = 0;
			for(int k = 0; k < 10; k++) {
				dyn[i][j] += mat[i][k] * mat[k][j];
			}
		}
	}

//	printf("\nOriginal, transposed\n");
//	for(int i = 0; i < 10; i++) {
//		for(int j = 0; j < 10; j++) {
//			printf("%d\t", mat[j][i]);
//		}
//		printf("\n");
//	}

	printf("\nExpected\n");
	for(int i = 0; i < 10; i++) {
		for(int j = 0; j < 10; j++) {
			printf("%f\t", dyn[j][i]);
		}
		printf("\n");
	}

	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 10, 10, 10, 1.0,
	    *cont, 10, *temp, 10, 0, *work, 10);

	printf("\nContiguous, transposed\n");
	for(int i = 0; i < 10; i++) {
		for(int j = 0; j < 10; j++) {
			printf("%f\t", work[j][i]);
		}
		printf("\n");
	}

	for(int i = 0; i < 10; i++) {
		free(dyn[i]);
	}

	free_mult_contig(2, cont, dyn);

	return (0);
}
