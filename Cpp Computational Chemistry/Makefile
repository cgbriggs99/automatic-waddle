
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
	
.PHONY: build_libs
build_libs: $(BASE_SOURCES) $(CHEM_SOURCES) $(CALC_SOURCES) ./Main/main.cpp

.PHONY: rebuild_libs
rebuild_libs:
	$(MAKE) -B build_libs
	
.PHONY:build_sources
build_sources: $(SOURCES)

.PHONY:rebuild_sources
rebuild_sources:
	$(MAKE) -B build_sources
	
calculations$(ARCH_SUFF): $(CALC_OBJECTS)
	$(AR) -cr $@ $^
chem$(ARCH_SUFF): $(CHEM_OBJECTS)
	$(AR) -cr $@ $^
base$(ARCH_SUFF): $(BASE_OBJECTS)
	$(AR) -cr $@ $^
	
$(SOURCES): $(DEPS)
	$(CC) $(CFLAGS) -o $(subst .cpp,$(EXEC_SUFF),$@) $@ $(DEPS) $(LDFLAGS) $(DLLS) $(DEFINES) $(INCLUDES)

$(BASE_OBJECTS) $(CHEM_OBJECTS) $(CALC_OBJECTS):
	$(CC) $(CFLAGS) -c -o $@ $(subst $(OBJ_SUFF),.cpp,$@) $(DEFINES) $(INCLUDES)
	
$(BASE_SOURCES) $(CHEM_SOURCES) $(CALC_SOURCES) ./Main/main.cpp:
	$(CC) $(CFLAGS) -c -o $(subst .cpp,$(OBJ_SUFF),$@) $@ $(DEFINES) $(INCLUDES)

	