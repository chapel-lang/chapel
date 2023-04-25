module cuBLAS {

  use C_CUBLAS;

  public use CTypes;

  record DevicePtr {
    type eltType;
    var val: c_ptr(eltType);
  }

  proc cublas_create_handle(){
    return cublas_create();
  }

  proc cublas_destroy_handle(handle: c_void_ptr){
    cublas_destroy(handle);
  }

  proc cu_array(size: c_size_t){
     require "c_cublas.h", "c_cublas.o";
     var x;
     x = cublas_array(size);
     return x;
  }

  proc cpu_to_gpu(src_ptr: c_ptr(?t), size: c_size_t){
    require "c_cublas.h", "c_cublas.o";
    var gpu_ptr: DevicePtr(t);
    gpu_ptr.val = to_gpu(src_ptr, size):c_void_ptr:c_ptr(t);
    return gpu_ptr;
  }

  proc gpu_to_cpu(dst_ptr: c_void_ptr, src_ptr: DevicePtr, size: c_size_t){
    require "c_cublas.h", "c_cublas.o";
    to_cpu(dst_ptr, src_ptr.val, size);
  }

  proc cu_axpy(handle: c_void_ptr, n: c_int, x: DevicePtr(c_float), y: DevicePtr(c_float), ref alpha: c_float, incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_saxpy(handle, n, alpha, x.val, incX, y.val, incY);
  }

  proc cu_axpy(handle: c_void_ptr, n: c_int, x: DevicePtr(c_double), y: DevicePtr(c_double), ref alpha: c_double, incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_daxpy(handle, n, alpha, x.val, incX, y.val, incY);
  } 

  proc cu_axpy(handle: c_void_ptr, n: c_int, x: DevicePtr(complex(64)), y: DevicePtr(complex(64)), ref alpha: complex(64), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_caxpy(handle, n, alpha, x.val, incX, y.val, incY);
  }

  proc cu_axpy(handle: c_void_ptr, n: c_int, x: DevicePtr(complex(128)), y: DevicePtr(complex(128)), ref alpha: complex(128), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_zaxpy(handle, n, alpha, x.val, incX, y.val, incY);
  }

  proc cu_iamax(handle: c_void_ptr, n: c_int, x: DevicePtr(c_float), incX: c_int, result: c_ptr(c_int)){
    require "c_cublas.h", "c_cublas.o";
    cublas_isamax(handle, n, x.val, incX, result);
  }

  proc cu_iamax(handle: c_void_ptr, n: c_int, x: DevicePtr(c_double), incX: c_int, result: c_ptr(c_int)){
    require "c_cublas.h", "c_cublas.o";
    cublas_idamax(handle, n, x.val, incX, result);
  }

  proc cu_iamax(handle: c_void_ptr, n: c_int, x: DevicePtr(complex(64)), incX: c_int, result: c_ptr(c_int)){
    require "c_cublas.h", "c_cublas.o";
    cublas_icamax(handle, n, x.val, incX, result);
  }

  proc cu_iamax(handle: c_void_ptr, n: c_int, x: DevicePtr(complex(128)), incX: c_int, result: c_ptr(c_int)){
    require "c_cublas.h", "c_cublas.o";
    cublas_izamax(handle, n, x.val, incX, result);
  }

  proc cu_iamin(handle: c_void_ptr, n: c_int, x: DevicePtr(c_float), incX: c_int, result: c_ptr(c_int)){
    require "c_cublas.h", "c_cublas.o";
    cublas_isamin(handle, n, x.val, incX, result);
  }

  proc cu_iamin(handle: c_void_ptr, n: c_int, x: DevicePtr(c_double), incX: c_int, result: c_ptr(c_int)){
    require "c_cublas.h", "c_cublas.o";
    cublas_idamin(handle, n, x.val, incX, result);
  }

  proc cu_iamin(handle: c_void_ptr, n: c_int, x: DevicePtr(complex(64)), incX: c_int, result: c_ptr(c_int)){
    require "c_cublas.h", "c_cublas.o";
    cublas_icamin(handle, n, x.val, incX, result);
  }

  proc cu_iamin(handle: c_void_ptr, n: c_int, x: DevicePtr(complex(128)), incX: c_int, result: c_ptr(c_int)){
    require "c_cublas.h", "c_cublas.o";
    cublas_izamin(handle, n, x.val, incX, result);
  }

  proc cu_asum(handle: c_void_ptr, n: c_int, x: DevicePtr(c_float), incX: c_int, result: c_ptr(c_float)){
    require "c_cublas.h", "c_cublas.o";
    cublas_sasum(handle, n, x.val, incX, result);
  }

  proc cu_asum(handle: c_void_ptr, n: c_int, x: DevicePtr(c_double), incX: c_int, result: c_ptr(c_double)){
    require "c_cublas.h", "c_cublas.o";
    cublas_dasum(handle, n, x.val, incX, result);
  }

  proc cu_asum(handle: c_void_ptr, n: c_int, x: DevicePtr(complex(64)), incX: c_int, result: c_ptr(c_float)){
    require "c_cublas.h", "c_cublas.o";
    cublas_scasum(handle, n, x.val, incX, result);
  }

  proc cu_asum(handle: c_void_ptr, n: c_int, x: DevicePtr(complex(128)), incX: c_int, result: c_ptr(c_double)){
    require "c_cublas.h", "c_cublas.o";
    cublas_dzasum(handle, n, x.val, incX, result);
  }

  proc cu_copy(handle: c_void_ptr, n: c_int, x: DevicePtr(c_float), y: DevicePtr(c_float), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_scopy(handle, n, x.val, incX, y.val, incY);
  }

  proc cu_copy(handle: c_void_ptr, n: c_int, x: DevicePtr(c_double), y: DevicePtr(c_double), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_dcopy(handle, n, x.val, incX, y.val, incY);
  }

  proc cu_copy(handle: c_void_ptr, n: c_int, x: DevicePtr(complex(64)), y: DevicePtr(complex(64)), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_ccopy(handle, n, x.val, incX, y.val, incY);
  }

  proc cu_copy(handle: c_void_ptr, n: c_int, x: DevicePtr(complex(128)), y: DevicePtr(complex(128)), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_zcopy(handle, n, x.val, incX, y.val, incY);
  }

  proc cu_dot(handle: c_void_ptr, n: c_int, x: DevicePtr(c_float), y: DevicePtr(c_float), result: c_ptr(c_float), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_sdot(handle, n, x.val, incX, y.val, incY, result);
  }

  proc cu_dot(handle: c_void_ptr, n: c_int, x: DevicePtr(c_double), y: DevicePtr(c_double), result: c_ptr(c_double), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_ddot(handle, n, x.val, incX, y.val, incY, result);
  }

  proc cu_dotu(handle: c_void_ptr, n: c_int, x: DevicePtr(complex(64)), y: DevicePtr(complex(64)), result: c_ptr(complex(64)), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_cdotu(handle, n, x.val, incX, y.val, incY, result);
  }

  proc cu_dotc(handle: c_void_ptr, n: c_int, x: DevicePtr(complex(64)), y: DevicePtr(complex(64)), result: c_ptr(complex(64)), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_cdotc(handle, n, x.val, incX, y.val, incY, result);
  }

  proc cu_dotu(handle: c_void_ptr, n: c_int, x: DevicePtr(complex(128)), y: DevicePtr(complex(128)), result: c_ptr(complex(128)), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_zdotu(handle, n, x.val, incX, y.val, incY, result);
  }

  proc cu_dotc(handle: c_void_ptr, n: c_int, x: DevicePtr(complex(128)), y: DevicePtr(complex(128)), result: c_ptr(complex(128)), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_zdotc(handle, n, x.val, incX, y.val, incY, result);
  }

  proc cu_nrm2(handle: c_void_ptr, n: c_int, x: DevicePtr(c_float), result: c_ptr(c_float), incX: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_snrm2(handle, n, x.val, incX, result);
  }

  proc cu_nrm2(handle: c_void_ptr, n: c_int, x: DevicePtr(c_double), result: c_ptr(c_double), incX: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_dnrm2(handle, n, x.val, incX, result);
  }

  proc cu_nrm2(handle: c_void_ptr, n: c_int, x: DevicePtr(complex(64)), result: c_ptr(c_float), incX: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_scnrm2(handle, n, x.val, incX, result);
  }

  proc cu_nrm2(handle: c_void_ptr, n: c_int, x: DevicePtr(complex(128)), result: c_ptr(c_double), incX: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_dznrm2(handle, n, x.val, incX, result);
  }

  proc cu_rot(handle: c_void_ptr, n: c_int, x: DevicePtr(c_float), y: DevicePtr(c_float), c: c_float, s: c_float, incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_srot(handle, n, x.val, incX, y.val, incY, c, s);
  }

  proc cu_rot(handle: c_void_ptr, n: c_int, x: DevicePtr(c_double), y: DevicePtr(c_double), c: c_double, s: c_double, incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_drot(handle, n, x.val, incX, y.val, incY, c, s);
  }

  proc cu_rot(handle: c_void_ptr, n: c_int, x: DevicePtr(complex(64)), y: DevicePtr(complex(64)), c: c_float, s: complex(64), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_crot(handle, n, x.val, incX, y.val, incY, c, s);
  }

  proc cu_rot(handle: c_void_ptr, n: c_int, x: DevicePtr(complex(64)), y: DevicePtr(complex(64)), c: c_float, s: c_float, incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_csrot(handle, n, x.val, incX, y.val, incY, c, s);
  }

  proc cu_rot(handle: c_void_ptr, n: c_int, x: DevicePtr(complex(128)), y: DevicePtr(complex(128)), c: c_double, s: complex(128), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_zrot(handle, n, x.val, incX, y.val, incY, c, s);
  }

  proc cu_rot(handle: c_void_ptr, n: c_int, x: DevicePtr(complex(128)), y: DevicePtr(complex(128)), c: c_double, s: c_double, incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_zdrot(handle, n, x.val, incX, y.val, incY, c, s);
  }

  proc cu_rotg(handle: c_void_ptr, a: c_float, b: c_float, ref c: c_float, ref s: c_float){
    require "c_cublas.h", "c_cublas.o";
    cublas_srotg(handle, a, b, c, s);
  }

  proc cu_rotg(handle: c_void_ptr, a: c_double, b: c_double, ref c: c_double, ref s: c_double){
    require "c_cublas.h", "c_cublas.o";
    cublas_drotg(handle, a, b, c, s);
  }

  proc cu_rotg(handle: c_void_ptr, a: complex(64), b: complex(64), ref c: c_float, ref s: complex(64)){
    require "c_cublas.h", "c_cublas.o";
    cublas_crotg(handle, a, b, c, s);
  }

  proc cu_rotg(handle: c_void_ptr, a: complex(128), b: complex(128), ref c: c_double, ref s: complex(128)){
    require "c_cublas.h", "c_cublas.o";
    cublas_zrotg(handle, a, b, c, s);
  }

  proc cu_rotm(handle: c_void_ptr, n: c_int, x: DevicePtr(c_float), y: DevicePtr(c_float), params: []real(32), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_srotm(handle, n, x.val, incX, y.val, incY, params);
  }

  proc cu_rotm(handle: c_void_ptr, n: c_int, x: DevicePtr(c_double), y: DevicePtr(c_double), params: []real(64), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_drotm(handle, n, x.val, incX, y.val, incY, params);
  }

  proc cu_rotmg(handle: c_void_ptr, ref d1: c_float, ref d2: c_float, ref x1: c_float, ref y1: c_float, params: []real(32)){
    require "c_cublas.h", "c_cublas.o";
    cublas_srotmg(handle, d1, d2, x1, y1, params);
  }

  proc cu_rotmg(handle: c_void_ptr, ref d1: c_double, ref d2: c_double, ref x1: c_double, ref y1: c_double, params: []real(64)){
    require "c_cublas.h", "c_cublas.o";
    cublas_drotmg(handle, d1, d2, x1, y1, params);
  }

  proc cu_scal(handle: c_void_ptr, n: c_int, alpha: c_float, x: DevicePtr(c_float), incX: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_sscal(handle, n, alpha, x.val, incX);
  }

  proc cu_scal(handle: c_void_ptr, n: c_int, alpha: c_double, x: DevicePtr(c_double), incX: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_dscal(handle, n, alpha, x.val, incX);
  }

  proc cu_scal(handle: c_void_ptr, n: c_int, alpha: complex(64), x: DevicePtr(complex(64)), incX: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_cscal(handle, n, alpha, x.val, incX);
  }

  proc cu_scal(handle: c_void_ptr, n: c_int, alpha: c_float, x: DevicePtr(complex(64)), incX: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_csscal(handle, n, alpha, x.val, incX);
  }

  proc cu_scal(handle: c_void_ptr, n: c_int, alpha: complex(128), x: DevicePtr(complex(128)), incX: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_zscal(handle, n, alpha, x.val, incX);
  }

  proc cu_scal(handle: c_void_ptr, n: c_int, alpha: c_double, x: DevicePtr(complex(128)), incX: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_zdscal(handle, n, alpha, x.val, incX);
  }

  proc cu_swap(handle: c_void_ptr, n: c_int, x: DevicePtr(c_float), y: DevicePtr(c_float), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_sswap(handle, n, x.val, incX, y.val, incY);
  }

  proc cu_swap(handle: c_void_ptr, n: c_int, x: DevicePtr(c_double), y: DevicePtr(c_double), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_dswap(handle, n, x.val, incX, y.val, incY);
  }

  proc cu_swap(handle: c_void_ptr, n: c_int, x: DevicePtr(complex(64)), y: DevicePtr(complex(64)), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_cswap(handle, n, x.val, incX, y.val, incY);
  }

  proc cu_swap(handle: c_void_ptr, n: c_int, x: DevicePtr(complex(128)), y: DevicePtr(complex(128)), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_zswap(handle, n, x.val, incX, y.val, incY);
  }

  proc cu_gemm(handle: c_void_ptr, transa: c_int, transb: c_int, m: c_int, n: c_int, k: c_int, alpha: c_float, A: DevicePtr(c_float), lda: c_int, B: DevicePtr(c_float), ldb: c_int, beta: c_float, c: DevicePtr(c_float), ldc: c_int){
    require "c_cublas.h", "c_cublas.o";
    cublas_sgemm(handle, transa, transb, m, n, k, alpha, A.val, lda, B.val, ldb, beta, c.val, ldc);
  }

/*
  proc cu_sgbmv(handle: c_void_ptr, trans: c_int, m: c_int, n: c_int, kl: c_int, ku: c_int, ref alpha: c_float, A: DevicePtr(c_float), lda: c_int, x: DevicePtr(c_float), incX: c_int, ref beta: c_float, y: DevicePtr(c_float), incY: c_int){
    require "c_cublas.h", "c_cublas.o";
    cublas_sgbmv(handle, m, n, kl, ku, alpha, A.val, lda, x.val, incX, beta, y.val, incY);
}

  proc cu_dgbmv(handle: c_void_ptr, trans: c_int, m: c_int, n: c_int, kl: c_int, ku: c_int, ref alpha: c_double, A: DevicePtr(c_double), lda: c_int, x: DevicePtr(c_double), incX: c_int, ref beta: c_double, y: DevicePtr(c_double), incY: c_int){
    require "c_cublas.h", "c_cublas.o";
    cublas_dgbmv(handle, m, n, kl, ku, alpha, A.val, lda, x.val, incX, beta, y.val, incY);
}

  proc cu_cgbmv(handle: c_void_ptr, trans: c_int, m: c_int, n: c_int, kl: c_int, ku: c_int, ref alpha: complex(64), A: DevicePtr(complex(64)), lda: c_int, x: DevicePtr(complex(64)), incX: c_int, ref beta: complex(64), y: DevicePtr(complex(64)), incY: c_int){
    require "c_cublas.h", "c_cublas.o";
    cublas_cgbmv(handle, m, n, kl, ku, alpha, A.val, lda, x.val, incX, beta, y.val, incY);
}

  proc cu_zgbmv(handle: c_void_ptr, trans: c_int, m: c_int, n: c_int, kl: c_int, ku: c_int, ref alpha: complex(128), A: DevicePtr(complex(128)), lda: c_int, x: DevicePtr(complex(128)), incX: c_int, ref beta: complex(128), y: DevicePtr(complex(128)), incY: c_int){
    require "c_cublas.h", "c_cublas.o";
    cublas_zgbmv(handle, m, n, kl, ku, alpha, A.val, lda, x.val, incX, beta, y.val, incY);
}
*/

  proc cu_gemv(handle: c_void_ptr, trans: c_int, m: c_int, n: c_int, ref alpha: c_float, A: DevicePtr(c_float), lda: c_int, x: DevicePtr(c_float), ref beta: c_float, y: DevicePtr(c_float), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_sgemv(handle, trans, m, n, alpha, A.val, lda, x.val, incX, beta, y.val, incY);
}

  proc cu_gemv(handle: c_void_ptr, trans: c_int, m: c_int, n: c_int, ref alpha: c_double, A: DevicePtr(c_double), lda: c_int, x: DevicePtr(c_double), ref beta: c_double, y: DevicePtr(c_double), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_dgemv(handle, trans, m, n, alpha, A.val, lda, x.val, incX, beta, y.val, incY);
}

  proc cu_gemv(handle: c_void_ptr, trans: c_int, m: c_int, n: c_int, ref alpha: complex(64), A: DevicePtr(complex(64)), lda: c_int, x: DevicePtr(complex(64)), ref beta: complex(64), y: DevicePtr(complex(64)), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_cgemv(handle, trans, m, n, alpha, A.val, lda, x.val, incX, beta, y.val, incY);
}

  proc cu_gemv(handle: c_void_ptr, trans: c_int, m: c_int, n: c_int, ref alpha: complex(128), A: DevicePtr(complex(128)), lda: c_int, x: DevicePtr(complex(128)), ref beta: complex(128), y: DevicePtr(complex(128)), incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_zgemv(handle, trans, m, n, alpha, A.val, lda, x.val, incX, beta, y.val, incY);
}

  proc cu_ger(handle: c_void_ptr, m: c_int, n: c_int, ref alpha: c_float, x: DevicePtr(c_float), y: DevicePtr(c_float), A: DevicePtr(c_float), lda: c_int, incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_sger(handle, m, n, alpha, x.val, incX, y.val, incY, A.val, lda);
}

  proc cu_ger(handle: c_void_ptr, m: c_int, n: c_int, ref alpha: c_double, x: DevicePtr(c_double), y: DevicePtr(c_double), A: DevicePtr(c_double), lda: c_int, incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_dger(handle, m, n, alpha, x.val, incX, y.val, incY, A.val, lda);
}

  proc cu_geru(handle: c_void_ptr, m: c_int, n: c_int, ref alpha: complex(64), x: DevicePtr(complex(64)), y: DevicePtr(complex(64)), A: DevicePtr(complex(64)), lda: c_int, incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_cgeru(handle, m, n, alpha, x.val, incX, y.val, incY, A.val, lda);
}

  proc cu_gerc(handle: c_void_ptr, m: c_int, n: c_int, ref alpha: complex(64), x: DevicePtr(complex(64)), y: DevicePtr(complex(64)), A: DevicePtr(complex(64)), lda: c_int, incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_cgrec(handle, m, n, alpha, x.val, incX, y.val, incY, A.val, lda);
}

  proc cu_geru(handle: c_void_ptr, m: c_int, n: c_int, ref alpha: complex(128), x: DevicePtr(complex(128)), y: DevicePtr(complex(128)), A: DevicePtr(complex(128)), lda: c_int, incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_zgeru(handle, m, n, alpha, x.val, incX, y.val, incY, A.val, lda);
}

  proc cu_gerc(handle: c_void_ptr, m: c_int, n: c_int, ref alpha: complex(128), x: DevicePtr(complex(128)), y: DevicePtr(complex(128)), A: DevicePtr(complex(128)), lda: c_int, incX: c_int = 1, incY: c_int = 1){
    require "c_cublas.h", "c_cublas.o";
    cublas_zgerc(handle, m, n, alpha, x.val, incX, y.val, incY, A.val, lda);
}

  module C_CUBLAS {

    use CTypes;

    //extern type complex = complex(64);

    extern proc cublas_create(): c_void_ptr;
    extern proc cublas_destroy(handle: c_void_ptr);
    extern proc cublas_array(size: c_size_t): c_ptr(c_float);
    extern proc to_gpu(src_ptr: c_void_ptr, size: c_size_t): c_ptr(c_float);
    extern proc to_cpu(dst_ptr: c_void_ptr, src_ptr: c_void_ptr, size: c_size_t): c_void_ptr;
 
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
