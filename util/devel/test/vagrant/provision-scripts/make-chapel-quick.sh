#!/usr/bin/env bash

cd chapel
source util/quickstart/setchplenv.bash
make
make check
