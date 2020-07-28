use cuBLAS;
use BLAS;
use Time;
use IO;

proc main(args: [] string) {

  var timer: Timer;
  var N = args[1]:int(32);

  type t = real(32);
  var a = 2: t;

  //allocate arrays
  var X = cu_array((N+1):size_t);
  var Y = cu_array((N+1):size_t);

  //put values in array
  for i in 0..N {
    X[i] = 3.0;
    Y[i] = 5.0;
  }

  //create cublas handle
  var cublas_handle = cublas_create_handle();

  //use cublas saxpy
  timer.start();
  cu_saxpy(cublas_handle, N:int(32), X, Y, a);
  timer.stop();

  //destroy cublas handle
  cublas_destroy_handle(cublas_handle);

  var e = timer.elapsed()*1000;

  var f = open("chpl_cublas_times.log", iomode.rw);
  var cout = f.openAppender();
  cout.write(e, "\n");

  writeln("cu_axpy, N=", N, ": ", e);
}

proc file.openAppender() {
  var writer = this.writer(start=this.size);
  return writer;
}
