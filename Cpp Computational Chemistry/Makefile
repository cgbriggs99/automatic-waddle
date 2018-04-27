
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
	
chem$(ARCH_SUFF): $(CHEM_OBJECTS)
	$(AR) -cr $@ $^
base$(ARCH_SUFF): $(BASE_OBJECTS)
	$(AR) -cr $@ $^
	
	
$(BASE_OBJECTS) $(CHEM_OBJECTS): $(BASE_SOURCES) $(CHEM_SOURCES)
	$(CC) $(CFLAGS) -c -o $@ $(subst $(OBJ_SUFF),.cpp,$@) $(DEFINES) $(INCLUDES)
	
$(BASE_SOURCES) $(CHEM_SOURCES):
#	$(CC) $(CFLAGS) -c -o $(subst .cpp,$(OBJ_SUFF),$@) $@ $(DEFINES) $(INCLUDES)
	