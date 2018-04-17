OPTIMIZE=-O3 -Ofast -faggressive-loop-optimizations -falign-functions -fauto-inc-dec -fbranch-count-reg\
-fbranch-probabilities -fbranch-target-load-optimize -fdce -fdelete-dead-exceptions -fdse -fexpensive-optimizations\
-ffinite-math-only -fforward-propagate -fif-conversion -fipa-cp

DEBUG=-DDEBUG -g3 -ggdb
EXCEL=$(DEBUG) -DEXCEL

LIBS=-L ../ -L ./ -lm -l memory -lmolecule -Xlinker -rpath=../ -Xlinker -rpath=./

CFLAGS=$(DEBUG) $(OTHER_ARGS)
ifneq ($(strip $(BLAS_LOC)),)
LIBS+= -L $(BLAS_LOC) -Xlinker -rpath=$(BLAS_LOC)
endif

ifneq ($(strip $(LAPACK_LOC)),)
LIBS+= -L $(LAPACK_LOC) -Xlinker -rpath=$(LAPACK_LOC)
endif

ifneq ($(strip $(CBLAS_LOC)),)
LIBS+= -L $(CBLAS_LOC) -Xlinker -rpath=$(CBLAS_LOC)
endif

ifneq ($(strip $(LAPACKE_LOC)),)
LIBS+= -L $(LAPACKE_LOC) -Xlinker -rpath=$(LAPACKE_LOC)
endif

ifneq ($(strip $(BLAS_NAME)),)
LIBS+= -l $(BLAS_NAME)
endif

ifneq ($(strip $(LAPACK_NAME)),)
LIBS+= -l $(LAPACK_NAME) 
endif

ifneq ($(strip $(CBLAS_NAME)),)
LIBS+= -l $(CBLAS_NAME)
endif

ifneq ($(strip $(LAPACKE_NAME)),)
LIBS+= -l $(LAPACKE_NAME)
endif


CFLAGS=$(DEBUG) $(OTHER_ARGS) -include $(CBLAS_HEADER) -include $(LAPACKE_HEADER)
export OPTIMIZE, DEBUG, EXCEL, CFLAGS