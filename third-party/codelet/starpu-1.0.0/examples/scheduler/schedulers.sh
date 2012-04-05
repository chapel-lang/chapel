#!/bin/sh

# StarPU --- Runtime system for heterogeneous multicore architectures.
#
# Copyright (C) 2012  Centre National de la Recherche Scientifique
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

check_success()
{
    if [ $1 -ne 0 ] ; then
	echo "failure" >&2
        exit $1
    fi
}

[ -x ./cholesky/cholesky_tag ] || exit 77

SCHEDULERS=`STARPU_SCHED="help" ./basic_examples/hello_world 2>&1 | awk '/->/ {print $1}'`

for sched in $SCHEDULERS
do
    echo "cholesky.$sched"
    STARPU_SCHED=$sched ./cholesky/cholesky_tag
    check_success $?
done
