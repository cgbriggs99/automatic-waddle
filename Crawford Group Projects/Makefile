
include ./local_build/paths.mk
include defs.mk
include targets.mk

PYTHON="C:/IntelPython3/python.exe"

#Other commands
CC=gcc

#This is the recipe to initialize the build environment. It works.
.PHONY: init_makefile
init_makefile:
	@echo $(PYTHON)
	$(PYTHON) generate_makefile.py

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

#This works.
$(SOURCES): $(EXEC_DEPS)
	$(CC) -o $(subst .c,$(EXEC_SUFF),$@) $@ $+ $(CFLAGS) $(LDFLAGS) $(DEFINES) $(INCLUDES)
	

	

