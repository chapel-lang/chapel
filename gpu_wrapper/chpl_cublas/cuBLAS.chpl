module cuBLAS {

  use C_CUBLAS;
  public use SysCTypes;

  proc cublas_create_handle(){
    return cublas_create();
  }

  proc cublas_destroy_handle(handle: c_void_ptr){
    cublas_destroy(handle);
  }

  proc cu_array(size: size_t){
     require "c_cublas.h", "c_cublas.o";
     var x;
     x = cublas_array(size);
     return x;
  }

  proc cu_axpy(handle: c_void_ptr, N: c_int, X: c_ptr(c_float), Y: c_ptr(c_float), ref alpha: c_float, incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_saxpy(handle, N, alpha, X, incX, Y, incY);
  }

  proc cu_sgemm(handle: c_void_ptr, transa: c_int, transb: c_int, m: c_int, n: c_int, k: c_int, alpha: c_float, A: c_ptr(c_float), lda: c_int, B: c_ptr(c_float), ldb: c_int, beta: c_float, c: c_ptr(c_float), ldc: c_int){
    require "c_cublas.h", "c_cublas.o";
    cublas_sgemm(handle, transa, transb, m, n, k, alpha, A, lda, B, ldb, beta, c, ldc);
  }

  module C_CUBLAS {
    use SysCTypes;
    extern proc cublas_create(): c_void_ptr;
    extern proc cublas_destroy(handle: c_void_ptr);
    extern proc cublas_array(size: size_t): c_ptr(c_float);
    extern proc cublas_saxpy(handle: c_void_ptr, N: c_int, alpha: c_float, X: c_ptr(c_float), incX: c_int, Y: c_ptr(c_float), incY: c_int);
    extern proc cublas_sgemm(handle: c_void_ptr, transa: c_int, transb: c_int, m: c_int, n: c_int, k: c_int, alpha: c_float, A: c_ptr(c_float), lda: c_int, B: c_ptr(c_float), ldb: c_int, beta: c_float, C: c_ptr(c_float), ldc: c_int);
  }

}
