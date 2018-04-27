
include base.mk
include paths.mk
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
	
chem$(ARCH_SUFF): $(CHEM_SOURCES)
	$(AR) -cr $@ $(subst .cpp,$(OBJ_SUFF),$(filter $(CHEM_SOURCES),$^))
base$(ARCH_SUFF): $(BASE_SOURCES)
	$(AR) -cr $@ $(subst .cpp,$(OBJ_SUFF),$(filter $(BASE_SOURCES),$^))
	
	
$(BASE_OBJECTS):
	$(CC) $(CFLAGS) -c -o $@ $(subst $(OBJ_SUFF),.cpp,$@) $(DEFINES) $(INCLUDES)
	
$(BASE_SOURCES):
	$(CC) $(CFLAGS) -c -o $(subst .cpp,$(OBJ_SUFF),$@) $@ $(DEFINES) $(INCLUDES)
	