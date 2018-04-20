include defs.mk

TARGETS=objs$(ARCH_SUFF) Geometry/geometry.c
ARCH_OBJS=./Memory/alloc$(OBJ_SUFF) ./Memory/contig$(OBJ_SUFF) ./Memory/free$(OBJ_SUFF) \
./Molecule/molecule$(OBJ_SUFF) ./Input_Output/input$(OBJ_SUFF) ./Input_Output/output$(OBJ_SUFF) \
./Calculations/calculations$(OBJ_SUFF) ./Calculations/calculations2$(OBJ_SUFF) ./Calculations/calculations3$(OBJ_SUFF) \
./Calculations/calculations4$(OBJ_SUFF)
EXECS=geometry
SOURCES=Geometry/geometry.c
OUTPUTS=objs$(ARCH_SUFF)

EXEC_DEPS=objs$(ARCH_SUFF)
LDFLAGS=-lm
CFLAGS=-g3 -ggdb -Og

L1_REMOVE=$(ARCH_OBJS) a.out
L2_REMOVE=$(L1_REMOVE) $(OUTPUTS)

export