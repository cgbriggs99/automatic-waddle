include base.mk
include chem.mk
include calculations.mk
include paths.mk
include defs.mk

ifeq ($(OS),Windows_NT)
CFLAGS=-Wall -Wextra -g
else
CFLAGS=-g3 -ggdb -Og
endif

EXECS=./Main/geometry$(EXEC_SUFF) ./Main/hessian$(EXEC_SUFF) ./Main/scf$(EXEC_SUFF) ./Main/mp2$(EXEC_SUFF)
SOURCES=./Main/geometry.cpp ./Main/hessian.cpp ./Main/scf.cpp ./Main/mp2.cpp

TARGETS=base$(ARCH_SUFF) chem$(ARCH_SUFF) calculations$(ARCH_SUFF) ./Main/main$(OBJ_SUFF) $(EXECS)
DEPS=base$(ARCH_SUFF) chem$(ARCH_SUFF) calculations$(ARCH_SUFF) ./Main/main$(OBJ_SUFF) 
OBJECTS=$(BASE_OBJECTS) $(CHEM_OBJECTS) $(CALC_OBJECTS) ./Main/main$(OBJ_SUFF)

DLLS=$(LAPACKE_LIB_PATH) $(CBLAS_LIB_PATH) $(LAPACK_LIB_PATH) $(BLAS_LIB_PATH) $(OTHER_LIBS)
LDFLAGS=$(LAPACKE_ARCH_PATH) $(CBLAS_ARCH_PATH) $(LAPACK_ARCH_PATH) $(BLAS_ARCH_PATH) $(OTHER_FLAGS) $(DLLS) -lm
DEFINES=-D'DLLS="$(subst ",\",$(DLLS))"'
INCLUDES=-I $(LAPACKE_HEADER_PATH) -I $(CBLAS_HEADER_PATH) -include $(LAPACKE_HEADER_NAME) -include $(CBLAS_HEADER_NAME)

ifeq ($(OS),Windows_NT)

L1_REMOVE=$(subst /,\\,$(ARCH_OBJS)) a.out
L2_REMOVE=$(L1_REMOVE) $(OUTPUTS)
else
L1_REMOVE=$(ARCH_OBJS) a.out
L2_REMOVE=$(L1_REMOVE) $(OUTPUTS)
endif
export