
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
	$(RM) *.obj

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
	
$(SOURCES): $(DEPS)
	$(CC) $(CFLAGS) -o $(subst .cpp,$(EXEC_SUFF),$@) $@ $(DEPS) $(LDFLAGS) $(DLLS) $(DEFINES) $(INCLUDES)

$(BASE_OBJECTS) $(CHEM_OBJECTS) $(CALC_OBJECTS):
	$(MAKE) $(subst $(OBJ_SUFF),.cpp,$@)
	
$(BASE_SOURCES) $(CHEM_SOURCES) $(CALC_SOURCES) ./Main/main.cpp:
	$(CC) $(CFLAGS) -c -o $(subst .cpp,$(OBJ_SUFF),$@) $@ $(DEFINES) $(INCLUDES)

	