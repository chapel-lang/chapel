###Compile

#### chpl + cuBLAS
compile `c_cublas.c`:
```nvcc -c c_cublas.c -lcublas```

compile `cuBLAS.chpl`:
```chpl cuBLAS.chpl -L/usr/local/cuda-10.2/lib64 -lcudart -lcublas```

compile `chpl_cublas_time.chpl`:
```chpl chpl_cublas_time.chpl -L/usr/local/cuda-10.2/lib64 -lcudart -lcublas```

#### c + cuBLAS
compile 'c_cublas_time.c`:
```nvcc -o c_cublas_time c_cublas_time.c -lcublas```

###Run

`chpl_cublas_time.chpl` performs cublas saxpy (y = a*x + y) on N values with chpl calling a cuBLAS wrapper. Run `chpl_cublas_time.chpl` with:

```./chpl_cublas_time.chpl <N>```

where N is the problem size.

`c_cublas_time.c` performs cublas saxpy (y = a*x + y) on N values with c calling a cuBLAS wrapper. Run `c_cublas_time.c` with:

```./c_cublas_time.c <N>```

where N is the problem size.


#Performance comparison between c+cuBLAS and chpl+cuBLAS

#### chpl + cuBLAS
create a log file `chpl_cublas_times.log` to record timings.

run chpl+cuBLAS on problem sizes 2 to 2^30 with:
```bash run_chpl_cublas_time.sh```

#### c + cuBLAS
run c+cuBLAS on problem sizes 2 to 2^30 with:
```bash run_c_cublas_time.sh```
