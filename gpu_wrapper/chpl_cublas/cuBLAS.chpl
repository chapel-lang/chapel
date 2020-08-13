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

  proc cpu_to_gpu(src_ptr: c_ptr, size: size_t){
    require "c_cublas.h", "c_cublas.o";
    var gpu_ptr;
    gpu_ptr = to_gpu(src_ptr, size);
    return gpu_ptr;
  }

  proc gpu_to_cpu(dst_ptr: c_void_ptr, src_ptr: c_void_ptr, size: size_t){
    require "c_cublas.h", "c_cublas.o";
    to_cpu(dst_ptr, src_ptr, size);
  }

  proc cu_saxpy(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), y: c_ptr(c_float), ref alpha: c_float, incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_saxpy(handle, n, alpha, x, incX, y, incY);
  }

  proc cu_daxpy(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), y: c_ptr(c_double), ref alpha: c_double, incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_daxpy(handle, n, alpha, x, incX, y, incY);
  } 

  proc cu_caxpy(handle: c_void_ptr, n: c_int, x: c_ptr(complex(64)), y: c_ptr(complex(64)), ref alpha: complex(64), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_caxpy(handle, n, alpha, x, incX, y, incY);
  }

  proc cu_zaxpy(handle: c_void_ptr, n: c_int, x: c_ptr(complex(128)), y: c_ptr(complex(128)), ref alpha: complex(128), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_zaxpy(handle, n, alpha, x, incX, y, incY);
  }

  proc cu_isamax(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), incX: c_int, result: c_ptr(c_int)){
    require "c_cublas.h", "c_cublas.o";
    cublas_isamax(handle, n, x, incX, result);
  }

  proc cu_idamax(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), incX: c_int, result: c_ptr(c_int)){
    require "c_cublas.h", "c_cublas.o";
    cublas_idamax(handle, n, x, incX, result);
  }

  proc cu_isamin(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), incX: c_int, result: c_ptr(c_int)){
    require "c_cublas.h", "c_cublas.o";
    cublas_isamin(handle, n, x, incX, result);
  }

  proc cu_idamin(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), incX: c_int, result: c_ptr(c_int)){
    require "c_cublas.h", "c_cublas.o";
    cublas_idamin(handle, n, x, incX, result);
  }

  proc cu_sasum(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), incX: c_int, result: c_ptr(c_float)){
    require "c_cublas.h", "c_cublas.o";
    cublas_sasum(handle, n, x, incX, result);
  }

  proc cu_dasum(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), incX: c_int, result: c_ptr(c_double)){
    require "c_cublas.h", "c_cublas.o";
    cublas_dasum(handle, n, x, incX, result);
  }

  proc cu_sgemm(handle: c_void_ptr, transa: c_int, transb: c_int, m: c_int, n: c_int, k: c_int, alpha: c_float, A: c_ptr(c_float), lda: c_int, B: c_ptr(c_float), ldb: c_int, beta: c_float, c: c_ptr(c_float), ldc: c_int){
    require "c_cublas.h", "c_cublas.o";
    cublas_sgemm(handle, transa, transb, m, n, k, alpha, A, lda, B, ldb, beta, c, ldc);
  }

  proc cu_scopy(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), y: c_ptr(c_float), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_scopy(handle, n, x, incX, y, incY);
  }

  proc cu_dcopy(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), y: c_ptr(c_double), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_dcopy(handle, n, x, incX, y, incY);
  }

  proc cu_sdot(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), y: c_ptr(c_float), result: c_ptr(c_float), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_sdot(handle, n, x, incX, y, incY, result);
  }

  proc cu_ddot(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), y: c_ptr(c_double), result: c_ptr(c_double), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_ddot(handle, n, x, incX, y, incY, result);
  }

  proc cu_snrm2(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), result: c_ptr(c_float), incX: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_snrm2(handle, n, x, incX, result);
  }

  proc cu_dnrm2(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), result: c_ptr(c_double), incX: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_dnrm2(handle, n, x, incX, result);
  }

  proc cu_srot(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), y: c_ptr(c_float), c: c_float, s: c_float, incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_srot(handle, n, x, incX, y, incY, c, s);
  }

  proc cu_drot(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), y: c_ptr(c_double), c: c_double, s: c_double, incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_drot(handle, n, x, incX, y, incY, c, s);
  }

  proc cu_srotg(handle: c_void_ptr, a: c_float, b: c_float, c: c_float, s: c_float){
    require "c_cublas.h", "c_cublas.o";
    cublas_srotg(handle, a, b, c, s);
  }

  proc cu_drotg(handle: c_void_ptr, a: c_double, b: c_double, c: c_double, s: c_double){
    require "c_cublas.h", "c_cublas.o";
    cublas_drotg(handle, a, b, c, s);
  }

  proc cu_srotm(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), y: c_ptr(c_float), params: c_ptr(c_float), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_srotm(handle, n, x, incX, y, incY, params);
  }

  proc cu_drotm(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), y: c_ptr(c_double), params: c_ptr(c_double), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_drotm(handle, n, x, incX, y, incY, params);
  }

  proc cu_srotmg(handle: c_void_ptr, d1: c_float, d2: c_float, x1: c_float, y1: c_float, params: c_ptr(c_float)){
    require "c_cublas.h", "c_cublas.o";
    cublas_srotmg(handle, d1, d2, x1, y1, params);
  }

  proc cu_drotmg(handle: c_void_ptr, d1: c_double, d2: c_double, x1: c_double, y1: c_double, params: c_ptr(c_float)){
    require "c_cublas.h", "c_cublas.o";
    cublas_drotmg(handle, d1, d2, x1, y1, params);
  }

  proc cu_sscal(handle: c_void_ptr, n: c_int, alpha: c_float, x: c_ptr(c_float), incX: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_sscal(handle, n, alpha, x, incX);
  }

  proc cu_dscal(handle: c_void_ptr, n: c_int, alpha: c_double, x: c_ptr(c_double), incX: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_dscal(handle, n, alpha, x, incX);
  }

  proc cu_sswap(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), y: c_ptr(c_float), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_sswap(handle, n, x, incX, y, incY);
  }

  proc cu_dswap(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), y: c_ptr(c_double), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_dswap(handle, n, x, incX, y, incY);
  }

  module C_CUBLAS {
    use SysCTypes;

    //extern type complex = complex(64);

    extern proc cublas_create(): c_void_ptr;
    extern proc cublas_destroy(handle: c_void_ptr);
    extern proc cublas_array(size: size_t): c_ptr(c_float);
    extern proc to_gpu(src_ptr: c_ptr, size: size_t): c_ptr(c_float);
    extern proc to_cpu(dst_ptr: c_void_ptr, src_ptr: c_void_ptr, size: size_t): c_void_ptr;
 
    extern proc cublas_saxpy(handle: c_void_ptr, n: c_int, alpha: c_float, x: c_ptr(c_float), incX: c_int, y: c_ptr(c_float), incY: c_int);
    extern proc cublas_daxpy(handle: c_void_ptr, n: c_int, alpha: c_double, x: c_ptr(c_double), incX: c_int, y: c_ptr(c_double), incY: c_int);
    extern proc cublas_caxpy(handle: c_void_ptr, n: c_int, alpha: complex(64), x: c_ptr(complex(64)), incX: c_int, y: c_ptr(complex(64)), incY: c_int);
    extern proc cublas_zaxpy(handle: c_void_ptr, n: c_int, alpha: complex(128), x: c_ptr(complex(128)), incX: c_int, y: c_ptr(complex(128)), incY: c_int);

    extern proc cublas_isamax(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), incX: c_int, result: c_ptr(c_int));
    extern proc cublas_idamax(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), incX: c_int, results: c_ptr(c_int));

    extern proc cublas_isamin(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), incX: c_int, result: c_ptr(c_int));
    extern proc cublas_idamin(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), incX: c_int, result: c_ptr(c_int));

    extern proc cublas_sasum(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), incX: c_int, result: c_ptr(c_float));
    extern proc cublas_dasum(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), incX: c_int, result: c_ptr(c_double));

    extern proc cublas_scopy(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), incX: c_int, y: c_ptr(c_float), incY: c_int);
    extern proc cublas_dcopy(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), incX: c_int, y: c_ptr(c_double), incY: c_int);

    extern proc cublas_sdot(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), incX: c_int, y: c_ptr(c_float), incY: c_int, result: c_ptr(c_float));
    extern proc cublas_ddot(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), incX: c_int, y: c_ptr(c_double), incY: c_int, result: c_ptr(c_double));

    extern proc cublas_snrm2(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), incX: c_int, result: c_ptr(c_float));
    extern proc cublas_dnrm2(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), incX: c_int, result: c_ptr(c_double));

    extern proc cublas_srot(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), incX: c_int, y: c_ptr(c_float), incY: c_int, c: c_float, s: c_float);
    extern proc cublas_drot(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), incX: c_int, y: c_ptr(c_double), incY: c_int, c: c_double, s: c_double);

    extern proc cublas_srotg(handle: c_void_ptr, a: c_float, b: c_float, c: c_float, s: c_float);
    extern proc cublas_drotg(handle: c_void_ptr, a: c_double, b: c_double, c: c_double, s: c_double);

    extern proc cublas_srotm(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), incX: c_int, y: c_ptr(c_float), incY: c_int, params: c_ptr(c_float));
    extern proc cublas_drotm(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), incX: c_int, y: c_ptr(c_double), incY: c_int, params: c_ptr(c_double));

    extern proc cublas_srotmg(handle: c_void_ptr, d1: c_float, d2: c_float, x1: c_float, y1: c_float, params: c_ptr(c_float));
    extern proc cublas_drotmg(handle: c_void_ptr, d1: c_double, d2: c_double, x1: c_double, y1: c_double, params: c_ptr(c_float));

    extern proc cublas_sscal(handle: c_void_ptr, n: c_int, alpha: c_float, x: c_ptr(c_float), incX: c_int);
    extern proc cublas_dscal(handle: c_void_ptr, n: c_int, alpha: c_double, x: c_ptr(c_double), incX: c_int);

    extern proc cublas_sswap(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), incX: c_int, y: c_ptr(c_float), incY: c_int);
    extern proc cublas_dswap(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), incX: c_int, y: c_ptr(c_double), incY: c_int);

    extern proc cublas_sgemm(handle: c_void_ptr, transa: c_int, transb: c_int, m: c_int, n: c_int, k: c_int, alpha: c_float, A: c_ptr(c_float), lda: c_int, B: c_ptr(c_float), ldb: c_int, beta: c_float, C: c_ptr(c_float), ldc: c_int);
  }

}
