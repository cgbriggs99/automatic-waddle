include defs.mk

TARGETS=objs$(ARCH_SUFF) Geometry/geometry.c
ARCH_OBJS=./Memory/alloc$(OBJ_SUFF) ./Memory/contig$(OBJ_SUFF) ./Memory/free$(OBJ_SUFF) \
./Molecule/molecule$(OBJ_SUFF) ./Input_Output/input$(OBJ_SUFF) ./Input_Output/output$(OBJ_SUFF) \
./Calculations/calculations$(OBJ_SUFF) ./Calculations/calculations2$(OBJ_SUFF) ./Calculations/calculations3$(OBJ_SUFF) \
./Calculations/calculations4$(OBJ_SUFF) ./Windows_Linking/linker$(OBJ_SUFF)
EXECS=geometry
SOURCES=Geometry/geometry.c
OUTPUTS=objs$(ARCH_SUFF)

EXEC_DEPS=objs$(ARCH_SUFF)
LDFLAGS=-lm
CFLAGS=-g3 -ggdb -Og
DLLS=$(LAPACK_LIB_PATH) $(BLAS_LIB_PATH) $(LAPACKE_LIB_PATH) $(CBLAS_LIB_PATH) $(OTHER_LIBS)

DEFINES=-D'DLLS=$(DLLS)'
INCLUDES=-I $(LAPACKE_HEADER_PATH) -I $(CBLAS_HEADER_PATH) -include $(LAPACKE_HEADER_NAME) -include $(CBLAS_HEADER_NAME)

L1_REMOVE=$(ARCH_OBJS) a.out
L2_REMOVE=$(L1_REMOVE) $(OUTPUTS)

export