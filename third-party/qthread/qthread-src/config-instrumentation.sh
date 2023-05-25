#!/bin/bash

qthread_root=`pwd`

cmocka_lib="${qthread_root}/dependencies/install/lib"
cmocka_inc="${qthread_root}/dependencies/install/include"

CFLAGS="-DQTHREAD_PERFORMANCE -DQTPERF_TESTING -I${cmocka_inc}" LDFLAGS="-L${cmocka_lib}" LIBS=-lcmocka ./configure --enable-debugging --disable-lazy-threadids  --enable-picky

## enable testing, disable perf debug output 
#CFLAGS='-DQTHREAD_PERFORMANCE -DQTPERF_TESTING' LDFLAGS=-L/home/erik/packages/lib LIBS=-lcmocka ./configure --enable-debugging --disable-lazy-threadids  --enable-picky

## Enable testing AND perfdbg output
#CFLAGS='-DQTHREAD_PERFORMANCE -DQTPERF_TESTING -DPERFDBG=1' ./configure --enable-debugging --disable-lazy-threadids  --enable-picky

## Disable testing, disable perfdbg output
#CFLAGS='-DQTHREAD_PERFORMANCE' ./configure --enable-debugging --disable-lazy-threadids  --enable-picky
