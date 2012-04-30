#!/usr/bin/python

import os

class bcolors:
    FAILURE = '\033[91m'
    NORMAL = '\033[0m'

def loadFunctionsAndDatatypes(flist, dtlist, fname):
    f = open(fname, 'r')
    for line in f:
        mline = line[:-1]
        if mline.count("@deftypefun "):
            if mline.count("fft") == 0:
                func = mline.replace("@deftypefun ", "").replace("*} ", "*").replace("@var{", "").replace("}", "").replace("{", "").replace(" (", "(", 1)
                flist.append(list([func, fname]))
        if mline.count("@deftp"):
            datatype = mline.replace("@deftp {Data Type} {", "").replace("}", "")
            dtlist.append(list([datatype, fname]))
    f.close()

functions = []
datatypes = []

loadFunctionsAndDatatypes(functions, datatypes, "doc/starpu.texi")
for docfile in os.listdir('doc/chapters'):
    if docfile.count(".texi"):
        loadFunctionsAndDatatypes(functions, datatypes, "doc/chapters/"+docfile)

for function in functions:
    x = os.system("fgrep -l \"" + function[0] + "\" include/*.h mpi/*.h starpufft/*h >foo")
    if x != 0:
        print "Function <" + bcolors.FAILURE + function[0] + bcolors.NORMAL + " > documented in <" + function[1] + "> does not exist in StarPU's API"

for datatype in datatypes:
    x = os.system("fgrep -l \"" + datatype[0] + "\" include/*.h mpi/*.h starpufft/*h >foo")
    if x != 0:
        print "Datatype <" + bcolors.FAILURE + datatype[0] + bcolors.NORMAL + "> documented in <" + datatype[1] + "> does not exist in StarPU's API"


