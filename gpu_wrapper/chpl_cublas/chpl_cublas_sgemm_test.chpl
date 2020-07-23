use cuBLAS;
use Time;
use IO;

proc main() {

  var N = 2:int(32);
  var alpha = 2:real(32);
  var beta = 2:real(32);

  //allocate arrays
  var X = cu_array((N*N):size_t);
  var Y = cu_array((N*N):size_t);
  var Z = cu_array((N*N):size_t);

  //put values in array
  for i in 0..N*N {
    X[i] = (i+1):real(32);
    Y[i] = (i+1):real(32);
    Z[i] = (i+1):real(32);
  }

  //create cublas handle
  var cublas_handle = cublas_create_handle();

  cu_sgemm(cublas_handle, 0, 0, N, N, N, alpha, X, N, Y, N, beta, Z, N);

  //destroy cublas handle
  cublas_destroy_handle(cublas_handle);


  for i in 0..N*N-1 {
    writeln(Z[i]);
  }
}
