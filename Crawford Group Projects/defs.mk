#Suffix for an archive.
ARCH_SUFF=.a
ifeq ($(OS),Windows_NT)
OBJ_SUFF=.obj
EXEC_SUFF=.exe
RM=del /f
else
#Suffix for an object file.
OBJ_SUFF=.o

#Suffix for an executable.
EXEC_SUFF=

#Removal command
RM=rm -rf
endif
export