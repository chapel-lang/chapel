#!/usr/bin/env bash

MAKEJ=${MAKEJ:=-j 4}

# could use $(chapel/util/buildRelease/chpl-make-cpu_count)}
# but that doesn't currently incorporate the amount of RAM per thread
