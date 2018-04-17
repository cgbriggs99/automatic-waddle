include paths.mk
include defs.mk


all:
	$(MAKE) -C ./Memory -I ../
	$(MAKE) -C ./Calculations -I ../
	$(MAKE) -C ./Molecule -I ../
	$(MAKE) -C ./Input_Output -I ../
	ld -r -soname libmolecule.dll calculations.o molecule.o io.o
	$(MAKE) -C ./Geometry -I ../
	$(MAKE) -C ./Harmonics -I ../
	$(MAKE) -C ./SCF -I ../
	$(MAKE) -C ./MP2 -I ../
	$(MAKE) -C ./CCSD -I ../
	$(MAKE) -C ./DIIS -I ../
#	$(MAKE) -C ./Symmetry
	
clean:
	del *.o