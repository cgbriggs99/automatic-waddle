
include paths.mk
include defs.mk
include targets.mk

#Other commands
CC=gcc

.PHONY: all
all: $(TARGETS)

.PHONY: rebuild
rebuild:
	$(MAKE) -B all


#This works.
.PHONY: clean
clean:
	$(RM) $(L1_REMOVE)
	$(RM) *.o
	$(RM) *.a

#This works.
.PHONY: purge
purge:
	$(MAKE) clean
	$(RM) $(L2_REMOVE)
	$(RM) *.exe
	$(RM) *.dll
	
#This works.
objs$(ARCH_SUFF): $(ARCH_OBJS)
	@echo Linking $@
	$(AR) -cr $@ $?
	
$(ARCH_CPP_OBJS): $(ARCH_CPP)
	@echo Building $@
	$(CC) -c -o $@ $(subst $(OBJ_SUFF),.cpp, $@) $(CFLAGS) $(DEFINES) $(INCLUDES)
	
$(filter-out $(ARCH_CPP_OBJS),$(ARCH_OBJS)): $(ARCH_C) 
	@echo Building $@
	$(CC) -c -o $@ $(subst $(OBJ_SUFF),.c, $@) $(CFLAGS) $(DEFINES) $(INCLUDES)

#This works.
$(SOURCES): $(EXEC_DEPS)
	@echo Building $@
	$(CC) -o $(notdir $(subst .c,$(EXEC_SUFF),$@)) $@ $+ $(CFLAGS) $(LDFLAGS) $(DEFINES) $(INCLUDES)

#$(EXECS): $(EXEC_DEPS)
#	$(CC) -o $@ $(subst $(EXEC_SUFF),.c,$@) $+ $(CFLAGS) $(LDFLAGS) $(DEFINES) $(INCLUDES)
	

	

