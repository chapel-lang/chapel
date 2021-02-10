#!/usr/bin/env bash

# TODO use `cc` on Cray-X*?
nvcc -c c_cublas.c -lcublas
