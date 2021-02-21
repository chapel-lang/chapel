module cuBLAS {

  use C_CUBLAS;
  public use CPtr;
  public use SysBasic;
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

  proc cu_icamax(handle: c_void_ptr, n: c_int, x: c_ptr(complex(64)), incX: c_int, result: c_ptr(c_int)){
    require "c_cublas.h", "c_cublas.o";
    cublas_icamax(handle, n, x, incX, result);
  }

  proc cu_izamax(handle: c_void_ptr, n: c_int, x: c_ptr(complex(128)), incX: c_int, result: c_ptr(c_int)){
    require "c_cublas.h", "c_cublas.o";
    cublas_izamax(handle, n, x, incX, result);
  }

  proc cu_isamin(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), incX: c_int, result: c_ptr(c_int)){
    require "c_cublas.h", "c_cublas.o";
    cublas_isamin(handle, n, x, incX, result);
  }

  proc cu_idamin(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), incX: c_int, result: c_ptr(c_int)){
    require "c_cublas.h", "c_cublas.o";
    cublas_idamin(handle, n, x, incX, result);
  }

  proc cu_icamin(handle: c_void_ptr, n: c_int, x: c_ptr(complex(64)), incX: c_int, result: c_ptr(c_int)){
    require "c_cublas.h", "c_cublas.o";
    cublas_icamin(handle, n, x, incX, result);
  }

  proc cu_izamin(handle: c_void_ptr, n: c_int, x: c_ptr(complex(128)), incX: c_int, result: c_ptr(c_int)){
    require "c_cublas.h", "c_cublas.o";
    cublas_izamin(handle, n, x, incX, result);
  }

  proc cu_sasum(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), incX: c_int, result: c_ptr(c_float)){
    require "c_cublas.h", "c_cublas.o";
    cublas_sasum(handle, n, x, incX, result);
  }

  proc cu_dasum(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), incX: c_int, result: c_ptr(c_double)){
    require "c_cublas.h", "c_cublas.o";
    cublas_dasum(handle, n, x, incX, result);
  }

  proc cu_scasum(handle: c_void_ptr, n: c_int, x: c_ptr(complex(64)), incX: c_int, result: c_ptr(c_float)){
    require "c_cublas.h", "c_cublas.o";
    cublas_scasum(handle, n, x, incX, result);
  }

  proc cu_dzasum(handle: c_void_ptr, n: c_int, x: c_ptr(complex(128)), incX: c_int, result: c_ptr(c_double)){
    require "c_cublas.h", "c_cublas.o";
    cublas_dzasum(handle, n, x, incX, result);
  }

  proc cu_scopy(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), y: c_ptr(c_float), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_scopy(handle, n, x, incX, y, incY);
  }

  proc cu_dcopy(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), y: c_ptr(c_double), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_dcopy(handle, n, x, incX, y, incY);
  }

  proc cu_ccopy(handle: c_void_ptr, n: c_int, x: c_ptr(complex(64)), y: c_ptr(complex(64)), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_ccopy(handle, n, x, incX, y, incY);
  }

  proc cu_zcopy(handle: c_void_ptr, n: c_int, x: c_ptr(complex(128)), y: c_ptr(complex(128)), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_zcopy(handle, n, x, incX, y, incY);
  }

  proc cu_sdot(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), y: c_ptr(c_float), result: c_ptr(c_float), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_sdot(handle, n, x, incX, y, incY, result);
  }

  proc cu_ddot(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), y: c_ptr(c_double), result: c_ptr(c_double), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_ddot(handle, n, x, incX, y, incY, result);
  }

  proc cu_cdotu(handle: c_void_ptr, n: c_int, x: c_ptr(complex(64)), y: c_ptr(complex(64)), result: c_ptr(complex(64)), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_cdotu(handle, n, x, incX, y, incY, result);
  }

  proc cu_cdotc(handle: c_void_ptr, n: c_int, x: c_ptr(complex(64)), y: c_ptr(complex(64)), result: c_ptr(complex(64)), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_cdotc(handle, n, x, incX, y, incY, result);
  }

  proc cu_zdotu(handle: c_void_ptr, n: c_int, x: c_ptr(complex(128)), y: c_ptr(complex(128)), result: c_ptr(complex(128)), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_zdotu(handle, n, x, incX, y, incY, result);
  }

  proc cu_zdotc(handle: c_void_ptr, n: c_int, x: c_ptr(complex(128)), y: c_ptr(complex(128)), result: c_ptr(complex(128)), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_zdotc(handle, n, x, incX, y, incY, result);
  }

  proc cu_snrm2(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), result: c_ptr(c_float), incX: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_snrm2(handle, n, x, incX, result);
  }

  proc cu_dnrm2(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), result: c_ptr(c_double), incX: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_dnrm2(handle, n, x, incX, result);
  }

  proc cu_scnrm2(handle: c_void_ptr, n: c_int, x: c_ptr(complex(64)), result: c_ptr(c_float), incX: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_scnrm2(handle, n, x, incX, result);
  }

  proc cu_dznrm2(handle: c_void_ptr, n: c_int, x: c_ptr(complex(128)), result: c_ptr(c_double), incX: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_dznrm2(handle, n, x, incX, result);
  }

  proc cu_srot(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), y: c_ptr(c_float), c: c_float, s: c_float, incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_srot(handle, n, x, incX, y, incY, c, s);
  }

  proc cu_drot(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), y: c_ptr(c_double), c: c_double, s: c_double, incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_drot(handle, n, x, incX, y, incY, c, s);
  }

  proc cu_crot(handle: c_void_ptr, n: c_int, x: c_ptr(complex(64)), y: c_ptr(complex(64)), c: c_float, s: complex(64), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_crot(handle, n, x, incX, y, incY, c, s);
  }

  proc cu_csrot(handle: c_void_ptr, n: c_int, x: c_ptr(complex(64)), y: c_ptr(complex(64)), c: c_float, s: c_float, incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_csrot(handle, n, x, incX, y, incY, c, s);
  }

  proc cu_zrot(handle: c_void_ptr, n: c_int, x: c_ptr(complex(128)), y: c_ptr(complex(128)), c: c_double, s: complex(128), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_zrot(handle, n, x, incX, y, incY, c, s);
  }

  proc cu_zdrot(handle: c_void_ptr, n: c_int, x: c_ptr(complex(128)), y: c_ptr(complex(128)), c: c_double, s: c_double, incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_zdrot(handle, n, x, incX, y, incY, c, s);
  }

  proc cu_srotg(handle: c_void_ptr, a: c_float, b: c_float, ref c: c_float, ref s: c_float){
    require "c_cublas.h", "c_cublas.o";
    cublas_srotg(handle, a, b, c, s);
  }

  proc cu_drotg(handle: c_void_ptr, a: c_double, b: c_double, ref c: c_double, ref s: c_double){
    require "c_cublas.h", "c_cublas.o";
    cublas_drotg(handle, a, b, c, s);
  }

  proc cu_crotg(handle: c_void_ptr, a: complex(64), b: complex(64), ref c: c_float, ref s: complex(64)){
    require "c_cublas.h", "c_cublas.o";
    cublas_crotg(handle, a, b, c, s);
  }

  proc cu_zrotg(handle: c_void_ptr, a: complex(128), b: complex(128), ref c: c_double, ref s: complex(128)){
    require "c_cublas.h", "c_cublas.o";
    cublas_zrotg(handle, a, b, c, s);
  }

  proc cu_srotm(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), y: c_ptr(c_float), params: []real(32), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_srotm(handle, n, x, incX, y, incY, params);
  }

  proc cu_drotm(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), y: c_ptr(c_double), params: []real(64), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_drotm(handle, n, x, incX, y, incY, params);
  }

  proc cu_srotmg(handle: c_void_ptr, ref d1: c_float, ref d2: c_float, ref x1: c_float, ref y1: c_float, params: []real(32)){
    require "c_cublas.h", "c_cublas.o";
    cublas_srotmg(handle, d1, d2, x1, y1, params);
  }

  proc cu_drotmg(handle: c_void_ptr, ref d1: c_double, ref d2: c_double, ref x1: c_double, ref y1: c_double, params: []real(64)){
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

  proc cu_cscal(handle: c_void_ptr, n: c_int, alpha: complex(64), x: c_ptr(complex(64)), incX: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_cscal(handle, n, alpha, x, incX);
  }

  proc cu_csscal(handle: c_void_ptr, n: c_int, alpha: c_float, x: c_ptr(complex(64)), incX: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_csscal(handle, n, alpha, x, incX);
  }

  proc cu_zscal(handle: c_void_ptr, n: c_int, alpha: complex(128), x: c_ptr(complex(128)), incX: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_zscal(handle, n, alpha, x, incX);
  }

  proc cu_zdscal(handle: c_void_ptr, n: c_int, alpha: c_double, x: c_ptr(complex(128)), incX: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_zdscal(handle, n, alpha, x, incX);
  }

  proc cu_sswap(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), y: c_ptr(c_float), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_sswap(handle, n, x, incX, y, incY);
  }

  proc cu_dswap(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), y: c_ptr(c_double), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_dswap(handle, n, x, incX, y, incY);
  }

  proc cu_cswap(handle: c_void_ptr, n: c_int, x: c_ptr(complex(64)), y: c_ptr(complex(64)), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_cswap(handle, n, x, incX, y, incY);
  }

  proc cu_zswap(handle: c_void_ptr, n: c_int, x: c_ptr(complex(128)), y: c_ptr(complex(128)), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_zswap(handle, n, x, incX, y, incY);
  }

  proc cu_sgemm(handle: c_void_ptr, transa: c_int, transb: c_int, m: c_int, n: c_int, k: c_int, alpha: c_float, A: c_ptr(c_float), lda: c_int, B: c_ptr(c_float), ldb: c_int, beta: c_float, c: c_ptr(c_float), ldc: c_int){
    require "c_cublas.h", "c_cublas.o";
    cublas_sgemm(handle, transa, transb, m, n, k, alpha, A, lda, B, ldb, beta, c, ldc);
  }

/*
  proc cu_sgbmv(handle: c_void_ptr, trans: c_int, m: c_int, n: c_int, kl: c_int, ku: c_int, ref alpha: c_float, A: c_ptr(c_float), lda: c_int, x: c_ptr(c_float), incX: c_int, ref beta: c_float, y: c_ptr(c_float), incY: c_int){
    require "c_cublas.h", "c_cublas.o";
    cublas_sgbmv(handle, m, n, kl, ku, alpha, A, lda, x, incX, beta, y, incY);
}

  proc cu_dgbmv(handle: c_void_ptr, trans: c_int, m: c_int, n: c_int, kl: c_int, ku: c_int, ref alpha: c_double, A: c_ptr(c_double), lda: c_int, x: c_ptr(c_double), incX: c_int, ref beta: c_double, y: c_ptr(c_double), incY: c_int){
    require "c_cublas.h", "c_cublas.o";
    cublas_dgbmv(handle, m, n, kl, ku, alpha, A, lda, x, incX, beta, y, incY);
}

  proc cu_cgbmv(handle: c_void_ptr, trans: c_int, m: c_int, n: c_int, kl: c_int, ku: c_int, ref alpha: complex(64), A: c_ptr(complex(64)), lda: c_int, x: c_ptr(complex(64)), incX: c_int, ref beta: complex(64), y: c_ptr(complex(64)), incY: c_int){
    require "c_cublas.h", "c_cublas.o";
    cublas_cgbmv(handle, m, n, kl, ku, alpha, A, lda, x, incX, beta, y, incY);
}

  proc cu_zgbmv(handle: c_void_ptr, trans: c_int, m: c_int, n: c_int, kl: c_int, ku: c_int, ref alpha: complex(128), A: c_ptr(complex(128)), lda: c_int, x: c_ptr(complex(128)), incX: c_int, ref beta: complex(128), y: c_ptr(complex(128)), incY: c_int){
    require "c_cublas.h", "c_cublas.o";
    cublas_zgbmv(handle, m, n, kl, ku, alpha, A, lda, x, incX, beta, y, incY);
}
*/

  proc cu_sgemv(handle: c_void_ptr, trans: c_int, m: c_int, n: c_int, ref alpha: c_float, A: c_ptr(c_float), lda: c_int, x: c_ptr(c_float), ref beta: c_float, y: c_ptr(c_float), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_sgemv(handle, trans, m, n, alpha, A, lda, x, incX, beta, y, incY);
}

  proc cu_dgemv(handle: c_void_ptr, trans: c_int, m: c_int, n: c_int, ref alpha: c_double, A: c_ptr(c_double), lda: c_int, x: c_ptr(c_double), ref beta: c_double, y: c_ptr(c_double), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_dgemv(handle, trans, m, n, alpha, A, lda, x, incX, beta, y, incY);
}

  proc cu_cgemv(handle: c_void_ptr, trans: c_int, m: c_int, n: c_int, ref alpha: complex(64), A: c_ptr(complex(64)), lda: c_int, x: c_ptr(complex(64)), ref beta: complex(64), y: c_ptr(complex(64)), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_cgemv(handle, trans, m, n, alpha, A, lda, x, incX, beta, y, incY);
}

  proc cu_zgemv(handle: c_void_ptr, trans: c_int, m: c_int, n: c_int, ref alpha: complex(128), A: c_ptr(complex(128)), lda: c_int, x: c_ptr(complex(128)), ref beta: complex(128), y: c_ptr(complex(128)), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_zgemv(handle, trans, m, n, alpha, A, lda, x, incX, beta, y, incY);
}

  proc cu_sger(handle: c_void_ptr, m: c_int, n: c_int, ref alpha: c_float, x: c_ptr(c_float), y: c_ptr(c_float), A: c_ptr(c_float), lda: c_int, incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_sger(handle, m, n, alpha, x, incX, y, incY, A, lda);
}

  proc cu_dger(handle: c_void_ptr, m: c_int, n: c_int, ref alpha: c_double, x: c_ptr(c_double), y: c_ptr(c_double), A: c_ptr(c_double), lda: c_int, incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_dger(handle, m, n, alpha, x, incX, y, incY, A, lda);
}

  proc cu_cgeru(handle: c_void_ptr, m: c_int, n: c_int, ref alpha: complex(64), x: c_ptr(complex(64)), y: c_ptr(complex(64)), A: c_ptr(complex(64)), lda: c_int, incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_cgeru(handle, m, n, alpha, x, incX, y, incY, A, lda);
}

  proc cu_cgerc(handle: c_void_ptr, m: c_int, n: c_int, ref alpha: complex(64), x: c_ptr(complex(64)), y: c_ptr(complex(64)), A: c_ptr(complex(64)), lda: c_int, incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_cgrec(handle, m, n, alpha, x, incX, y, incY, A, lda);
}

  proc cu_zgeru(handle: c_void_ptr, m: c_int, n: c_int, ref alpha: complex(128), x: c_ptr(complex(128)), y: c_ptr(complex(128)), A: c_ptr(complex(128)), lda: c_int, incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_zgeru(handle, m, n, alpha, x, incX, y, incY, A, lda);
}

  proc cu_zgerc(handle: c_void_ptr, m: c_int, n: c_int, ref alpha: complex(128), x: c_ptr(complex(128)), y: c_ptr(complex(128)), A: c_ptr(complex(128)), lda: c_int, incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_zgerc(handle, m, n, alpha, x, incX, y, incY, A, lda);
}

  module C_CUBLAS {
    use CPtr;
    use SysBasic;
    use SysCTypes;

    //extern type complex = complex(64);

    extern proc cublas_create(): c_void_ptr;
    extern proc cublas_destroy(handle: c_void_ptr);
    extern proc cublas_array(size: size_t): c_ptr(c_float);
    extern proc to_gpu(src_ptr: c_void_ptr, size: size_t): c_ptr(c_float);
    extern proc to_cpu(dst_ptr: c_void_ptr, src_ptr: c_void_ptr, size: size_t): c_void_ptr;
 
    extern proc cublas_saxpy(handle: c_void_ptr, n: c_int, alpha: c_float, x: c_ptr(c_float), incX: c_int, y: c_ptr(c_float), incY: c_int);
    extern proc cublas_daxpy(handle: c_void_ptr, n: c_int, alpha: c_double, x: c_ptr(c_double), incX: c_int, y: c_ptr(c_double), incY: c_int);
    extern proc cublas_caxpy(handle: c_void_ptr, n: c_int, alpha: complex(64), x: c_ptr(complex(64)), incX: c_int, y: c_ptr(complex(64)), incY: c_int);
    extern proc cublas_zaxpy(handle: c_void_ptr, n: c_int, alpha: complex(128), x: c_ptr(complex(128)), incX: c_int, y: c_ptr(complex(128)), incY: c_int);

    extern proc cublas_isamax(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), incX: c_int, result: c_ptr(c_int));
    extern proc cublas_idamax(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), incX: c_int, results: c_ptr(c_int));
    extern proc cublas_icamax(handle: c_void_ptr, n: c_int, x: c_ptr(complex(64)), incX: c_int, result: c_ptr(c_int));
    extern proc cublas_izamax(handle: c_void_ptr, n: c_int, x: c_ptr(complex(128)), incX: c_int, results: c_ptr(c_int));

    extern proc cublas_isamin(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), incX: c_int, result: c_ptr(c_int));
    extern proc cublas_idamin(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), incX: c_int, result: c_ptr(c_int));
    extern proc cublas_icamin(handle: c_void_ptr, n: c_int, x: c_ptr(complex(64)), incX: c_int, result: c_ptr(c_int));
    extern proc cublas_izamin(handle: c_void_ptr, n: c_int, x: c_ptr(complex(128)), incX: c_int, result: c_ptr(c_int));

    extern proc cublas_sasum(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), incX: c_int, result: c_ptr(c_float));
    extern proc cublas_dasum(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), incX: c_int, result: c_ptr(c_double));
    extern proc cublas_scasum(handle: c_void_ptr, n: c_int, x: c_ptr(complex(64)), incX: c_int, result: c_ptr(c_float));
    extern proc cublas_dzasum(handle: c_void_ptr, n: c_int, x: c_ptr(complex(128)), incX: c_int, result: c_ptr(c_double));

    extern proc cublas_scopy(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), incX: c_int, y: c_ptr(c_float), incY: c_int);
    extern proc cublas_dcopy(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), incX: c_int, y: c_ptr(c_double), incY: c_int);
    extern proc cublas_ccopy(handle: c_void_ptr, n: c_int, x: c_ptr(complex(64)), incX: c_int, y: c_ptr(complex(64)), incY: c_int);
    extern proc cublas_zcopy(handle: c_void_ptr, n: c_int, x: c_ptr(complex(128)), incX: c_int, y: c_ptr(complex(128)), incY: c_int);

    extern proc cublas_sdot(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), incX: c_int, y: c_ptr(c_float), incY: c_int, result: c_ptr(c_float));
    extern proc cublas_ddot(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), incX: c_int, y: c_ptr(c_double), incY: c_int, result: c_ptr(c_double));
    extern proc cublas_cdotu(handle: c_void_ptr, n: c_int, x: c_ptr(complex(64)), incX: c_int, y: c_ptr(complex(64)), incY: c_int, result: c_ptr(complex(64)));
    extern proc cublas_cdotc(handle: c_void_ptr, n: c_int, x: c_ptr(complex(64)), incX: c_int, y: c_ptr(complex(64)), incY: c_int, result: c_ptr(complex(64)));
    extern proc cublas_zdotu(handle: c_void_ptr, n: c_int, x: c_ptr(complex(128)), incX: c_int, y: c_ptr(complex(128)), incY: c_int, result: c_ptr(complex(128)));
    extern proc cublas_zdotc(handle: c_void_ptr, n: c_int, x: c_ptr(complex(128)), incX: c_int, y: c_ptr(complex(128)), incY: c_int, result: c_ptr(complex(128)));

    extern proc cublas_snrm2(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), incX: c_int, result: c_ptr(c_float));
    extern proc cublas_dnrm2(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), incX: c_int, result: c_ptr(c_double));
    extern proc cublas_scnrm2(handle: c_void_ptr, n: c_int, x: c_ptr(complex(64)), incX: c_int, result: c_ptr(c_float));
    extern proc cublas_dznrm2(handle: c_void_ptr, n: c_int, x: c_ptr(complex(128)), incX: c_int, result: c_ptr(c_double));

    extern proc cublas_srot(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), incX: c_int, y: c_ptr(c_float), incY: c_int, c: c_float, s: c_float);
    extern proc cublas_drot(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), incX: c_int, y: c_ptr(c_double), incY: c_int, c: c_double, s: c_double);
    extern proc cublas_crot(handle: c_void_ptr, n: c_int, x: c_ptr(complex(64)), incX: c_int, y: c_ptr(complex(64)), incY: c_int, c: c_float, s: complex(64));
    extern proc cublas_csrot(handle: c_void_ptr, n: c_int, x: c_ptr(complex(64)), incX: c_int, y: c_ptr(complex(64)), incY: c_int, c: c_float, s: c_float);
    extern proc cublas_zrot(handle: c_void_ptr, n: c_int, x: c_ptr(complex(128)), incX: c_int, y: c_ptr(complex(128)), incY: c_int, c: c_double, s: complex(128));
    extern proc cublas_zdrot(handle: c_void_ptr, n: c_int, x: c_ptr(complex(128)), incX: c_int, y: c_ptr(complex(128)), incY: c_int, c: c_double, s: c_double);

    extern proc cublas_srotg(handle: c_void_ptr, a: c_float, b: c_float, ref c: c_float, ref s: c_float);
    extern proc cublas_drotg(handle: c_void_ptr, a: c_double, b: c_double, ref c: c_double, ref s: c_double);
    extern proc cublas_crotg(handle: c_void_ptr, a: complex(64), b: complex(64), ref c: c_float, ref s: complex(64));
    extern proc cublas_zrotg(handle: c_void_ptr, a: complex(128), b: complex(128), ref c: c_double, ref s: complex(128));

    extern proc cublas_srotm(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), incX: c_int, y: c_ptr(c_float), incY: c_int, params: []c_float);
    extern proc cublas_drotm(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), incX: c_int, y: c_ptr(c_double), incY: c_int, params: []c_double);

    extern proc cublas_srotmg(handle: c_void_ptr, ref d1: c_float, ref d2: c_float, ref x1: c_float, ref y1: c_float, params: []c_float);
    extern proc cublas_drotmg(handle: c_void_ptr, ref d1: c_double, ref d2: c_double, ref x1: c_double, ref y1: c_double, params: []c_double);

    extern proc cublas_sscal(handle: c_void_ptr, n: c_int, alpha: c_float, x: c_ptr(c_float), incX: c_int);
    extern proc cublas_dscal(handle: c_void_ptr, n: c_int, alpha: c_double, x: c_ptr(c_double), incX: c_int);
    extern proc cublas_cscal(handle: c_void_ptr, n: c_int, alpha: complex(64), x: c_ptr(complex(64)), incX: c_int);
    extern proc cublas_csscal(handle: c_void_ptr, n: c_int, alpha: c_float, x: c_ptr(complex(64)), incX: c_int);
    extern proc cublas_zscal(handle: c_void_ptr, n: c_int, alpha: complex(128), x: c_ptr(complex(128)), incX: c_int);
    extern proc cublas_zdscal(handle: c_void_ptr, n: c_int, alpha: c_double, x: c_ptr(complex(128)), incX: c_int);

    extern proc cublas_sswap(handle: c_void_ptr, n: c_int, x: c_ptr(c_float), incX: c_int, y: c_ptr(c_float), incY: c_int);
    extern proc cublas_dswap(handle: c_void_ptr, n: c_int, x: c_ptr(c_double), incX: c_int, y: c_ptr(c_double), incY: c_int);
    extern proc cublas_cswap(handle: c_void_ptr, n: c_int, x: c_ptr(complex(64)), incX: c_int, y: c_ptr(complex(64)), incY: c_int);
    extern proc cublas_zswap(handle: c_void_ptr, n: c_int, x: c_ptr(complex(128)), incX: c_int, y: c_ptr(complex(128)), incY: c_int);

/*
    extern proc cublas_sgbmv(handle: c_void_ptr, trans: c_int, m: c_int, n: c_int, kl: c_int, ku: c_int, ref alpha: c_float, A: c_ptr(c_float), lda: c_int, x: c_ptr(c_float), incX: c_int, ref beta: c_float, y: c_ptr(c_float), incY: c_int);
    extern proc cublas_dgbmv(handle: c_void_ptr, trans: c_int, m: c_int, n: c_int, kl: c_int, ku: c_int, ref alpha: c_double, A: c_ptr(c_double), lda: c_int, x: c_ptr(c_double), incX: c_int, ref beta: c_double, y: c_ptr(c_double), incY: c_int);
    extern proc cublas_cgbmv(handle: c_void_ptr, trans: c_int, m: c_int, n: c_int, kl: c_int, ku: c_int, ref alpha: complex(64), A: c_ptr(complex(64)), lda: c_int, x: c_ptr(complex(64)), incX: c_int, ref beta: complex(64), y: c_ptr(complex(64)), incY: c_int);
    extern proc cublas_zgbmv(handle: c_void_ptr, trans: c_int, m: c_int, n: c_int, kl: c_int, ku: c_int, ref alpha: complex(128), A: c_ptr(complex(128)), lda: c_int, x: c_ptr(complex(128)), incX: c_int, ref beta: complex(128), y: c_ptr(complex(128)), incY: c_int);
*/

    extern proc cublas_sgemv(handle: c_void_ptr, trans: c_int, m: c_int, n: c_int, alpha: c_float, A: c_ptr(c_float), lda: c_int, x: c_ptr(c_float), incX: c_int, beta: c_float, y: c_ptr(c_float), incY: c_int);
    extern proc cublas_dgemv(handle: c_void_ptr, trans: c_int, m: c_int, n: c_int, alpha: c_double, A: c_ptr(c_double), lda: c_int, x: c_ptr(c_double), incX: c_int, beta: c_double, y: c_ptr(c_double), incY: c_int);
    extern proc cublas_cgemv(handle: c_void_ptr, trans: c_int, m: c_int, n: c_int, alpha: complex(64), A: c_ptr(complex(64)), lda: c_int, x: c_ptr(complex(64)), incX: c_int, beta: complex(64), y: c_ptr(complex(64)), incY: c_int);
    extern proc cublas_zgemv(handle: c_void_ptr, trans: c_int, m: c_int, n: c_int, alpha: complex(128), A: c_ptr(complex(128)), lda: c_int, x: c_ptr(complex(128)), incX: c_int, beta: complex(128), y: c_ptr(complex(128)), incY: c_int);

    extern proc cublas_sger(handle: c_void_ptr, m: c_int, n: c_int, alpha: c_float, x: c_ptr(c_float), incX: c_int, y: c_ptr(c_float), incY: c_int, A: c_ptr(c_float), lda: c_int);
    extern proc cublas_dger(handle: c_void_ptr, m: c_int, n: c_int, alpha: c_double, x: c_ptr(c_double), incX: c_int, y: c_ptr(c_double), incY: c_int, A: c_ptr(c_double), lda: c_int);
    extern proc cublas_cgeru(handle: c_void_ptr, m: c_int, n: c_int, alpha: complex(64), x: c_ptr(complex(64)), incX: c_int, y: c_ptr(complex(64)), incY: c_int, A: c_ptr(complex(64)), lda: c_int);
extern proc cublas_cgerc(handle: c_void_ptr, m: c_int, n: c_int, alpha: complex(64), x: c_ptr(complex(64)), incX: c_int, y: c_ptr(complex(64)), incY: c_int, A: c_ptr(complex(64)), lda: c_int);
    extern proc cublas_zgeru(handle: c_void_ptr, m: c_int, n: c_int, alpha: complex(128), x: c_ptr(complex(128)), incX: c_int, y: c_ptr(complex(128)), incY: c_int, A: c_ptr(complex(128)), lda: c_int);
    extern proc cublas_zgerc(handle: c_void_ptr, m: c_int, n: c_int, alpha: complex(128), x: c_ptr(complex(128)), incX: c_int, y: c_ptr(complex(128)), incY: c_int, A: c_ptr(complex(128)), lda: c_int);


    extern proc cublas_sgemm(handle: c_void_ptr, transa: c_int, transb: c_int, m: c_int, n: c_int, k: c_int, alpha: c_float, A: c_ptr(c_float), lda: c_int, B: c_ptr(c_float), ldb: c_int, beta: c_float, C: c_ptr(c_float), ldc: c_int);
  }

}
