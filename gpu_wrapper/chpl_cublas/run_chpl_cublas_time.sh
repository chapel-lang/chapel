#!/bin/bash

X=1
N=30

for (( VAR=1; VAR<=$N; VAR++ )) do
    X=$(($X*2))
    ./chpl_cublas_time $X
done

