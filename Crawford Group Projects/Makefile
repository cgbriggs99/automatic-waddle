
include ./local_build/paths.mk
include defs.mk
include targets.mk

#Other commands
CC=gcc

.PHONY: all
all: $(TARGETS)

.PHONY: rebuild
rebuild:
	$(MAKE) -B all
<<<<<<< HEAD
=======
	
.PHONY: subtest
subtest:
	@echo $(SOURCES)
	@echo $(shell cd)
	@echo $(shell stringmod.bat $(SOURCES))
>>>>>>> refs/remotes/origin/master

#This works.
.PHONY: clean
clean:
	$(RM) $(L1_REMOVE)
	$(RM) *.o
	$(RM) *.a

#This works.
.PHONY: purge
purge:
	$(RM) $(L2_REMOVE)
	
#This works.
objs$(ARCH_SUFF): $(ARCH_OBJS)
	$(AR) -cr $@ $?
	
#This works.
$(ARCH_OBJS): $(ARCH_OBJS:$(OBJ_SUFF)=.c)
	$(CC) -c -o $@ $(subst $(OBJ_SUFF),.c, $@) $(CFLAGS) $(DEFINES) $(INCLUDES) 

#This works.
$(SOURCES): $(EXEC_DEPS)
	$(CC) -o $(patsubst ,,$(subst .c,$(EXEC_SUFF),$@)) $@ $+ $(CFLAGS) $(LDFLAGS) $(DEFINES) $(INCLUDES)

#$(EXECS): $(EXEC_DEPS)
#	$(CC) -o $@ $(subst $(EXEC_SUFF),.c,$@) $+ $(CFLAGS) $(LDFLAGS) $(DEFINES) $(INCLUDES)
	

	

