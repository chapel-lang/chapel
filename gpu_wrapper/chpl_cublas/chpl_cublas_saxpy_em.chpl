use cuBLAS;
use BLAS;
use Time;
use IO;

proc main() {

  var timer: Timer;
  var N = 10:int(32);

  type t = real(32);
  var a = 2: t;

  //allocate arrays
  var X: [1..N] real(32);
  var Y: [1..N] real(32);

  //put values in arrays
  X = 3.0:real(32);
  Y = 5.0:real(32);

  var gpu_ptr_X = cpu_to_gpu(c_ptrTo(X), c_sizeof(real)*N:size_t);
  var gpu_ptr_Y = cpu_to_gpu(c_ptrTo(Y), c_sizeof(real)*N:size_t);


  //create cublas handle
  var cublas_handle = cublas_create_handle();

  //use cublas saxpy
  timer.start();
  cu_axpy(cublas_handle, N:int(32), gpu_ptr_X, gpu_ptr_Y, a);
  gpu_to_cpu(c_ptrTo(Y), gpu_ptr_Y, c_sizeof(real)*N:size_t); 
  timer.stop();

  //destroy cublas handle
  cublas_destroy_handle(cublas_handle);

  var e = timer.elapsed()*1000;

  for i in 1..N {
    writeln(Y[i]);
  }

}

proc file.openAppender() {
  var writer = this.writer(start=this.size);
  return writer;
}
