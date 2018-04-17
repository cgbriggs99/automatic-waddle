OPTIMIZE=-O3 -Ofast -faggressive-loop-optimizations -falign-functions -fauto-inc-dec -fbranch-count-reg\
-fbranch-probabilities -fbranch-target-load-optimize -fdce -fdelete-dead-exceptions -fdse -fexpensive-optimizations\
-ffinite-math-only -fforward-propagate -fif-conversion -fipa-cp

DEBUG=-DDEBUG -g3 -ggdb
EXCEL=$(DEBUG) -DEXCEL

LIBS=-lm -lmemory

CFLAGS=$(DEBUG) $(OTHER_ARGS)
ifneq ($(strip $(BLAS_LOC)),)
CFLAGS+= -L $(BLAS_LOC)
endif

ifneq ($(strip $(LAPACK_LOC)),)
CFLAGS+= -L $(LAPACK_LOC) 
endif

ifneq ($(strip $(CBLAS_LOC)),)
CFLAGS+= -L $(CBLAS_LOC)
endif

ifneq ($(strip $(LAPACKE_LOC)),)
CFLAGS+= -L $(LAPACKE_LOC)
endif


CFLAGS+=-l $(LAPACKE_NAME) -l $(CBLAS_NAME) -l $(LAPACK_NAME) -l $(BLAS_NAME) -include $(CBLAS_HEADER) -include $(LAPACKE_HEADER)
export OPTIMIZE, DEBUG, EXCEL, CFLAGS