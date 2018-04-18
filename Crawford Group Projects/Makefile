include paths.mk
include defs.mk


all:
	$(MAKE) -B -C ./Memory
	$(MAKE) -B -C ./Calculations
	$(MAKE) -B -C ./Molecule
	$(MAKE) -B -C ./Input_Output
	ld -r -soname molecule.dll calculations.o molecule.o io.o
	$(MAKE) -C ./Geometry
#	$(MAKE) -C ./Harmonics
#	$(MAKE) -C ./SCF
#	$(MAKE) -C ./MP2
#	$(MAKE) -C ./CCSD
#	$(MAKE) -C ./DIIS
#	$(MAKE) -C ./Symmetry
	
clean:
	del *.o