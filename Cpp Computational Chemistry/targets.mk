include base.mk
include chem.mk
include paths.mk
include defs.mk

ifeq ($(OS),Windows_NT)
CFLAGS=-g
else
CFLAGS=-g3 -ggdb -Og
endif

TARGETS=base$(ARCH_SUFF) chem$(ARCH_SUFF) $(SOURCES)


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