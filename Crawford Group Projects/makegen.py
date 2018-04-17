#!/bin/python3

import os
import sys

# Default directories and names
__cblas_h = "cblas.h"
__lapacke_h = "lapacke.h"
__cblas_lib = "cblas"
__lapacke_lib = "lapacke"
__blas_lib = "blas"
__lapack_lib = "lapack"

#Windows


def __setup_make(fd : int):
    print("# This file was generated automatically.", file = fd)
    
def __write_others(fd, compiler):
    print("CC=%s"%(compiler), file = fd)
    
def __write_libs(fd, blas_path, lapack_path, cblas_path, lapacke_path, blas_name, lapack_name, cblas_name, lapacke_name, others):
    print("BLAS_LOC=%s"%(blas_path), file = fd)
    print("LAPACK_LOC=%s"%(lapack_path), file = fd)
    print("BLAS_NAME=%s"%(blas_name), file = fd)
    print("LAPACK_NAME=%s"%(lapack_name), file = fd)
    print("CBLAS_LOC=%s"%(cblas_path), file = fd)
    print("LAPACKE_LOC=%s"%(lapacke_path), file = fd)
    print("CBLAS_NAME=%s"%(cblas_name), file = fd)
    print("LAPACKE_NAME=%s"%(lapacke_name), file = fd)
    print("OTHER_ARGS=%s"%(others), file = fd)
    
def __write_includes(fd, blas_name, lapack_name):
    print("CBLAS_HEADER=%s"%(blas_name), file = fd)
    print("LAPACKE_HEADER=%s"%(lapack_name), file = fd)

def __write_export(fd):
    print("export", file = fd)
    
def generate():
    try :
        paths = open("paths.mk", "x")
        print("'paths.mk' created successfully.")
    except FileExistsError:
        print("Could not create file 'paths.mk'; deleting.")
        print("os.remove('paths.mk')")
        try :
            os.remove("paths.mk")
            print("'paths.mk' successfully deleted.")
        except:
            print("File could not be removed.")
            exit()
        paths = open("paths.mk", "x")
        print("'paths.mk' created successfully.")
    compiler = input("Enter the compiler command: ");
    blas_lib = input("BLAS library name (default 'blas'): ")
    blas_lib_path = input("BLAS library location (default to check path): ")
    lapack_lib = input("LAPACK library name (default 'lapack'): ");
    lapack_lib_path = input("LAPACK library location (default to check path): ")
    cblas_lib = input("CBLAS library name (default 'cblas'): ");
    cblas_lib_path = input("CBLAS library location (default to check path): ")
    cblas_header = input("CBLAS header file full name (default to 'cblas.h'): ")
    lapacke_lib = input("LAPACKE library name (default 'lapacke'): ");
    lapacke_lib_path = input("LAPACKE library location (default to check path): ")
    lapacke_header = input("LAPACKE header file full name (default to 'lapacke.h'): ")
    others = input("Any dependencies or other arguments: ")
    
    if blas_lib == '':
        blas_lib = __blas_lib
    if lapack_lib == '':
        lapack_lib = __lapack_lib
    if cblas_lib == '':
        cblas_lib = __cblas_lib
    if lapacke_lib == '':
        lapacke_lib = __lapacke_lib
    if cblas_header == '':
        cblas_header = __cblas_h
    if lapacke_header == '':
        lapacke_header = __lapacke_h
    
    __setup_make(paths)
    __write_others(paths, compiler)
    __write_libs(paths, blas_lib_path, lapack_lib_path, cblas_lib_path, lapacke_lib_path, blas_lib, lapack_lib, cblas_lib, lapacke_lib, others)
    __write_includes(paths, cblas_header, lapacke_header)
    __write_export(paths)
    print("'paths.mk' successfully formatted.")
    paths.close()
    
generate()
