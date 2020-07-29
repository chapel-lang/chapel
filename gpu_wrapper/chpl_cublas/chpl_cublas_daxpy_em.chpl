use cuBLAS;
use BLAS;
use Time;
use IO;

proc main() {

  var timer: Timer;
  var N = 10:int(32);

  type t = real(64);
  var a = 2: t;

  //allocate arrays
  var X: [1..N] real(64);
  var Y: [1..N] real(64);

  //put values in arrays
  X = 3.0: real(64);
  Y = 5.0: real(64);

  var gpu_ptr_X = cpu_to_gpu(c_ptrTo(X), c_sizeof(real(64))*N:size_t);
  var gpu_ptr_Y = cpu_to_gpu(c_ptrTo(Y), c_sizeof(real(64))*N:size_t);


  //create cublas handle
  var cublas_handle = cublas_create_handle();

  //use cublas saxpy
  cu_daxpy(cublas_handle, N, gpu_ptr_X:c_ptr(real(64)), gpu_ptr_Y:c_ptr(real(64)), a);
  gpu_to_cpu(c_ptrTo(Y), gpu_ptr_Y, c_sizeof(real)*N:size_t); 

  //destroy cublas handle
  cublas_destroy_handle(cublas_handle);

  for i in 1..N {
    writeln(Y[i]);
  }

}
