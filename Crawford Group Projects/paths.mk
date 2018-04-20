#This makefile is for the various paths and names specific to your computer.

#Full names of the BLAS and LAPACK libraries.
LAPACK_LIB_PATH=/home/connor/lapack-Sandbox/lapack-final/lib/liblapack.so
BLAS_LIB_PATH=/home/connor/lapack-Sandbox/lapack-final/lib/libblas.so
LAPACKE_LIB_PATH=/home/connor/lapack-Sandbox/lapack-final/lib/liblapacke.so
CBLAS_LIB_PATH=/home/connor/lapack-Sandbox/lapack-final/lib/libcblas.so

#Library names
OTHER_LIBS=
OTHER_FLAGS=-lgfortran -Xlinker -rpath=/home/connor/lapack-Sandbox/lapack-final/lib/

#Header names and locations
LAPACKE_HEADER_PATH=/home/connor/lapack-Sandbox/lapack-final/include
LAPACKE_HEADER_NAME=lapacke.h
CBLAS_HEADER_PATH=/home/connor/lapack-Sandbox/lapack-final/include
CBLAS_HEADER_NAME=cblas.h
export