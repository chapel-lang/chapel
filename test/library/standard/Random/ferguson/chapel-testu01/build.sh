#!/bin/bash

bash get-and-make-third-party.sh

source env.sh

chpl --fast chplu01.chpl  -L $TESTU01_LIB -I $TESTU01_INC -stestReal=true -stestRealBits=32 -suseNPB=true -o test-npb32
chpl --fast chplu01.chpl  -L $TESTU01_LIB -I $TESTU01_INC -stestReal=true -stestRealBits=64 -suseNPB=true -o test-npb64

chpl --fast chplu01.chpl  -L $TESTU01_LIB -I $TESTU01_INC -stestReal=true -suseNPB=false -stestRealBits=32 -o test-pcg-real-32
chpl --fast chplu01.chpl  -L $TESTU01_LIB -I $TESTU01_INC -stestReal=true -suseNPB=false -stestRealBits=64 -o test-pcg-real-64

chpl --fast chplu01.chpl  -L $TESTU01_LIB -I $TESTU01_INC -stestReal=false -suseNPB=false -stestUintBits=32 -o test-pcg-uint-32
chpl --fast chplu01.chpl  -L $TESTU01_LIB -I $TESTU01_INC -stestReal=false -suseNPB=false -stestUintBits=64 -o test-pcg-uint-64

chpl --fast chplu01.chpl  -L $TESTU01_LIB -I $TESTU01_INC -stestReal=false -suseNPB=false -stestUintBits=32 -stestBoundedRand=true -o test-pcg-uint-32-bounded

gcc -O3 pcg-c-crush.c  $PCG_SRC/pcg_basic.c -I $PCG_SRC/  -L $TESTU01_LIB -I $TESTU01_INC -ltestu01 -lprobdist -lmylib -o test-pcg-uint-32-c
