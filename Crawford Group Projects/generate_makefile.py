#!/bin/python3

# This python script sets up the build environment, setting things like the operating system
# and artifact prefixes and suffixes.

import os
import re 

def write_makefile(fs) :
    fs.seek(0);
    curr_os = os.name
    matches = 0
    arch_suff = '.a'
    if curr_os == 'nt' :
        obj_suff = '.obj'
        exec_suff = '.exe'
        rm_command = 'del /f'
    else :
        obj_suff = '.o'
        exec_suff = ''
        rm_command = 'rm -rf'
        
    
    #Get the position of the start of the line.
    pos1 = fs.tell()
    x = None
    while x != '' :
        pos1 = fs.tell()
        x = fs.readline()
        if re.match("^CURR_OS=", x) != None :
            #Get the rest of the file so that we can do an insert.
            rest = fs.read()
    
            fs.seek(pos1)
            fs.write("CURR_OS=%s\n"%(curr_os))
            pos2 = fs.tell()
            fs.write(rest)
            fs.seek(pos2)
            matches += 1
        elif re.match("^ARCH_SUFF=", x) != None :
            #Get the rest of the file so that we can do an insert.
            rest = fs.read()
    
            fs.seek(pos1)
            fs.write("ARCH_SUFF=%s\n"%(arch_suff))
            pos2 = fs.tell()
            fs.write(rest)
            fs.seek(pos2)
            matches += 1
        elif re.match("^OBJ_SUFF=", x) != None :
            #Get the rest of the file so that we can do an insert.
            rest = fs.read()
    
            fs.seek(pos1)
            fs.write("OBJ_SUFF=%s\n"%(obj_suff))
            pos2 = fs.tell()
            fs.write(rest)
            fs.seek(pos2)
            matches += 1
        elif re.match("^EXEC_SUFF=", x) != None :
            #Get the rest of the file so that we can do an insert.
            rest = fs.read()
    
            fs.seek(pos1)
            fs.write("EXEC_SUFF=%s\n"%(exec_suff))
            pos2 = fs.tell()
            fs.write(rest)
            fs.seek(pos2)
            matches += 1
        elif re.match("^RM=", x) != None :
            #Get the rest of the file so that we can do an insert.
            rest = fs.read()
    
            fs.seek(pos1)
            fs.write("RM=%s\n"%(rm_command))
            pos2 = fs.tell()
            fs.write(rest)
            fs.seek(pos2)
            matches += 1    
    if matches != 4 :
        fs.close()
        os.remove("./local_build/defs.mk")
        fs = init_defs()
        write_makefile(fs)


def init_defs():
    try :
        defs_mk = open("./local_build/defs.mk", "r+")
    except FileNotFoundError :
        defs_copy = open("Default Files/defs.mk")
        fd = os.open("./local_build/defs.mk", os.O_CREAT | os.O_RDWR)
        defs_mk = os.fdopen(fd, "r+")
        defs_mk.write(paths_copy.read())
        defs_copy.close()
    return defs_mk

def init_paths():
    try :
        paths = open("./local_build/paths.mk", "r")
    except FileNotFoundError :
        paths_copy = open("Default Files/paths.mk")
        fd = os.open("./local_build/paths.mk", os.O_CREAT | os.O_RDWR)
        paths = os.fdopen(fd, "r+")
        paths.write(paths_copy.read())
        paths_copy.close()
    paths.close()

if not os.path.exists("./local_build") :
    os.mkdirs("./local_build")
defs_mk = init_defs()
init_paths()
write_makefile(defs_mk)
defs_mk.close()
