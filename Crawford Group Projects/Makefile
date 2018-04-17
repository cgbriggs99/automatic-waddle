include defs.mk


all:
	$(MAKE) -C ./Calculations
	$(MAKE) -C ./Molecule
	$(MAKE) -C ./Input_Output
	ld -r -o libmolecule.dll calculations.o molecule.o io.o
	$(MAKE) -C ./Geometry
	$(MAKE) -C ./Harmonics
	$(MAKE) -C ./SCF
	$(MAKE) -C ./MP2
	$(MAKE) -C ./CCSD
	$(MAKE) -C ./DIIS
#	$(MAKE) -C ./Symmetry
	
clean:
	del *.o