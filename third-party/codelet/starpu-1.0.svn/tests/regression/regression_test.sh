#!/bin/bash

# StarPU --- Runtime system for heterogeneous multicore architectures.
# 
# Copyright (C) 2008, 2009, 2010  Université de Bordeaux 1
# Copyright (C) 2010  Centre National de la Recherche Scientifique
# 
# StarPU is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2.1 of the License, or (at
# your option) any later version.
# 
# StarPU is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# 
# See the GNU Lesser General Public License in COPYING.LGPL for more details.


DIR=$PWD
ROOTDIR=$DIR/../..
COVDIR=coverage
MAXCPU=3

init()
{
	mkdir -p $COVDIR
	lcov --directory $COVDIR --zerocounters
}

save_cov()
{
	testname=$1
	lcov --directory $ROOTDIR --capture --output $COVDIR/$testname.info 
	lcov -a $COVDIR/$testname.info -o $COVDIR/all.info
}

generatehtml()
{
	cd $COVDIR
	genhtml all.info
	cd $DIR
}

cd $ROOTDIR
make clean 1> /dev/null 2> /dev/null
make examples -j STARPU_ATLAS=1 CPUS=$MAXCPU CUDA=1 COVERAGE=1 1> /dev/null 2> /dev/null
cd $DIR

init;

echo "heat.dm.8k.no.pin.v2"
timing=`STARPU_SCHED="dm" $ROOTDIR/examples/heat/heat -ntheta 66 -nthick 130 -nblocks 8 -v2 2> log`
save_cov "heat.dm.8k.no.pin.v2";

echo "heat.prio.8k"
timing=`STARPU_SCHED="prio" $ROOTDIR/examples/heat/heat -ntheta 66 -nthick 130 -nblocks 8 -v2 -pin 2> log`
save_cov "heat.prio.8k";

echo "heat.dm.8k.v2"
timing=`STARPU_SCHED="dm" $ROOTDIR/examples/heat/heat -ntheta 66 -nthick 130 -nblocks 8 -pin -v2 2> log`
save_cov "heat.dm.8k.v2";

echo "heat.ws.8k.v2"
timing=`STARPU_SCHED="ws" $ROOTDIR/examples/heat/heat -ntheta 66 -nthick 130 -nblocks 8 -pin -v2 2> log`
save_cov "heat.ws.8k.v2";

echo "heat.greedy.8k.v2"
timing=`STARPU_SCHED="greedy" $ROOTDIR/examples/heat/heat -ntheta 66 -nthick 130 -nblocks 8 -pin -v2 2> log`
save_cov "heat.greedy.8k.v2";

echo "heat.dm.8k.cg"
timing=`$ROOTDIR/examples/heat/heat -ntheta 66 -nthick 130 -nblocks 8 -pin -v2 -cg 2> log`
save_cov "heat.dm.8k.cg";

echo "heat.dm.8k.v3"
timing=`STARPU_SCHED="dm" $ROOTDIR/examples/heat/heat -ntheta 66 -nthick 130 -nblocks 8 -pin -v3 2> log`
save_cov "heat.dm.8k.v3";

echo "incrementer"
timing=`$ROOTDIR/examples/incrementer/incrementer 2> log`
save_cov "incrementer";

generatehtml;
