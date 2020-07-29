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

  proc cu_caxpy(handle: c_void_ptr, n: c_int, ref alpha: complex(64), x: c_ptr(complex(64)), incX: c_int = 1, y: c_ptr(complex(64)), incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cubas_caxpy(handle, n, alpha, x, incX, y, incY);
  }

  proc cu_zaxpy(handle: c_void_ptr, n: c_int, ref alpha: complex(128), x: c_ptr(complex(128)), incX: c_int = 1, y: c_ptr(complex(128)), incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cubas_zaxpy(handle, n, alpha, x, incX, y, incY);
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
    extern proc to_gpu(src_ptr: c_ptr, size: size_t): c_ptr(c_float);
    extern proc to_cpu(dst_ptr: c_void_ptr, src_ptr: c_void_ptr, size: size_t): c_void_ptr;
 
    extern proc cublas_saxpy(handle: c_void_ptr, N: c_int, alpha: c_float, X: c_ptr(c_float), incX: c_int, Y: c_ptr(c_float), incY: c_int);
    extern proc cublas_daxpy(handle: c_void_ptr, N: c_int, alpha: c_double, X: c_ptr(c_double), incX: c_int, Y: c_ptr(c_double), incY: c_int);
    extern proc cublas_caxpy(handle: c_void_ptr, N: c_int, alpha: c_float, X: c_ptr(c_float), incX: c_int, Y: c_ptr(c_float), incY: c_int);
    extern proc cublas_caxpy(handle: c_void_ptr, N: c_int, alpha: c_float, X: c_ptr(c_double), incX: c_int, Y: c_ptr(c_double), incY: c_int);


    extern proc cublas_sgemm(handle: c_void_ptr, transa: c_int, transb: c_int, m: c_int, n: c_int, k: c_int, alpha: c_float, A: c_ptr(c_float), lda: c_int, B: c_ptr(c_float), ldb: c_int, beta: c_float, C: c_ptr(c_float), ldc: c_int);
  }

}
