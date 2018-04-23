#!/bin/python3

__names = []
__symbols = []
__masses = []

def mass(name = None, number = None, symbol = None):
    if name != None :
        try :
            ind = __names.index(name.lower())
            return __masses[ind]
        except ValueError :
            pass
    
    if number != None :
        return __masses[number - 1]
    
    if symbol != None :
        try :
            ind = __symbols.index(symbol)
            return __masses[ind]
        except ValueError :
            raise ValueError("Could not find the element.")