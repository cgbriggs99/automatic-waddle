
include base.mk
include paths.mk
include calculations.mk
include defs.mk
include targets.mk

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
	
calculations$(ARCH_SUFF): $(CALC_OBJECTS)
	$(AR) -cr $@ $^
chem$(ARCH_SUFF): $(CHEM_OBJECTS)
	$(AR) -cr $@ $^
base$(ARCH_SUFF): $(BASE_OBJECTS)
	$(AR) -cr $@ $^
	
$(EXECS): $(SOURCES) $(DEPS)
	$(CC) $(CFLAGS) -o $@ $(subst $(EXEC_SUFF),.cpp,$@) $(OBJECTS) $(LDFLAGS) $(DLLS) $(DEFINES) $(INCLUDES)

$(BASE_OBJECTS) $(CHEM_OBJECTS) $(CALC_OBJECTS) ./Main/main$(OBJ_SUFF): $(BASE_SOURCES) $(CHEM_SOURCES) $(CALC_SOURCES) ./Main/main.cpp
	$(CC) $(CFLAGS) -c -o $@ $(subst $(OBJ_SUFF),.cpp,$@) $(DEFINES) $(INCLUDES)
	
$(BASE_SOURCES) $(CHEM_SOURCES) $(CALC_SOURCES) $(SOURCES) ./Main/main.cpp:
	