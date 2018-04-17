include paths.mk

OPTIMIZE=-O3 -Ofast -faggressive-loop-optimizations -falign-functions -fauto-inc-dec -fbranch-count-reg\
-fbranch-probabilities -fbranch-target-load-optimize -fdce -fdelete-dead-exceptions -fdse -fexpensive-optimizations\
-ffinite-math-only -fforward-propagate -fif-conversion -fipa-cp

DEBUG=-DDEBUG -g3 -ggdb
EXCEL=$(DEBUG) -DEXCEL

CFLAGS+=$(DEBUG)
export OPTIMIZE, DEBUG, EXCEL, CFLAGS