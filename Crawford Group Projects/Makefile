include paths.mk
include defs.mk
include targets.mk

#This is the python command to use to initialize the build environment.
PYTHON=python3

#Other commands
CC=gcc

#This is the recipe to initialize the build environment. It works.
.PHONY: init_makefile
init_makefile:
	$(PYTHON) ./generate_makefile.py
	

.PHONY: all
all: $(TARGETS)

#This works.
.PHONY: clean
clean:
	$(RM) $(L1_REMOVE)

#This works.
.PHONY: purge
purge:
	$(RM) $(L2_REMOVE)
	
#This works.
objs$(ARCH_SUFF): $(ARCH_OBJS)
	$(AR) -cr $@ $?
	
#This works.
$(ARCH_OBJS): $(ARCH_OBJS:$(OBJ_SUFF)=.c)
	$(CC) -c -o $@ $(subst $(OBJ_SUFF),.c, $@) $(DEFINES) $(INCLUDES) 

#Compiles fine. Now complete the command line.
$(SOURCES): $(EXEC_DEPS)
	$(CC) -o $(subst .c,$(EXEC_SUFF),$@) $@ $+ $(CFLAGS) $(LDFLAGS) $(DEFINES) $(INCLUDES)
	@echo $@


	

