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

  module C_CUBLAS {
    use SysCTypes;
    extern proc cublas_create(): c_void_ptr;
    extern proc cublas_destroy(handle: c_void_ptr);
    extern proc cublas_array(size: size_t): c_ptr(c_float);
    extern proc cublas_saxpy(handle: c_void_ptr, N: c_int, alpha: c_float, X: c_ptr(c_float), incX: c_int, Y: c_ptr(c_float), incY: c_int);
  }

}
