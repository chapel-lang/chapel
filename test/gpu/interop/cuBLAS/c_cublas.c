#include <cuda_runtime.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include  "cublas_v2.h"

void* to_gpu(void* src_ptr, size_t size){
    void *dst_ptr;
    cudaError_t malloc_err = cudaMalloc(&dst_ptr, size);
    //printf("CUDA malloc error: %s\n", cudaGetErrorString(malloc_err));
    cudaError_t memcpy_err = cudaMemcpy(dst_ptr, src_ptr, size, cudaMemcpyHostToDevice);
    //printf("CUDA memcpy HtoD error: %s\n", cudaGetErrorString(memcpy_err));
    return dst_ptr;

}

void to_cpu(void* dst_ptr, void* src_ptr, size_t size){

    cudaError_t memcpy_err = cudaMemcpy(dst_ptr, src_ptr, size, cudaMemcpyDeviceToHost);
    //printf("CUDA memcpy DtoH error: %s\n", cudaGetErrorString(memcpy_err));

}

void* cublas_create(void){

    cublasHandle_t *handle = (cublasHandle_t*)malloc(sizeof(cublasHandle_t));
    cublasStatus_t err = cublasCreate(handle);
    //printf("cublasCreate status: %s\n", cudaGetErrorString(err));
    return handle;

}

void cublas_destroy(cublasHandle_t *handle){

    cublasStatus_t err = cublasDestroy(*handle);
    //printf("cublasDestroy status: %s\n", cudaGetErrorString(err));

}

int cublas_saxpy(cublasHandle_t *handle, int n, float alpha, float *x, int incX, float *y, int incY){

    cublasSaxpy(*handle, n, &alpha, x, incX, y, incY);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_daxpy(cublasHandle_t *handle, int n, double alpha, double *x, int incX, double *y, int incY){

    cublasDaxpy(*handle, n, &alpha, x, incX, y, incY);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_caxpy(cublasHandle_t *handle, int n, complex float alpha, complex float *x, int incX, complex float *y, int incY){

    cublasCaxpy(*handle, n, (cuFloatComplex*) &alpha, (cuFloatComplex*) x,  incX, (cuFloatComplex*) y, incY);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_zaxpy(cublasHandle_t *handle, int n, complex double alpha, complex double *x, int incX, complex double *y, int incY){

    cublasZaxpy(*handle, n, (cuDoubleComplex*) &alpha, (cuDoubleComplex*) x, incX, (cuDoubleComplex*) y, incY);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_isamax(cublasHandle_t *handle, int n, float *x, int incX, int *results){

    cublasIsamax(*handle, n, x, incX, results);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_idamax(cublasHandle_t *handle, int n, double *x, int incX, int *results){

    cublasIdamax(*handle, n, x, incX, results);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_icamax(cublasHandle_t *handle, int n, complex float *x, int incX, int *results){

    cublasIcamax(*handle, n, (cuFloatComplex*) x, incX, results);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_izamax(cublasHandle_t *handle, int n, complex double *x, int incX, int *results){

    cublasIzamax(*handle, n, (cuDoubleComplex*) x, incX, results);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_isamin(cublasHandle_t *handle, int n, float *x, int incX, int *results){

    cublasIsamin(*handle, n, x, incX, results);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_idamin(cublasHandle_t *handle, int n, double *x, int incX, int *results){

    cublasIdamin(*handle, n, x, incX, results);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_icamin(cublasHandle_t *handle, int n, complex float *x, int incX, int *results){

    cublasIcamin(*handle, n, (cuFloatComplex*) x, incX, results);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_izamin(cublasHandle_t *handle, int n, complex double *x, int incX, int *results){

    cublasIzamin(*handle, n, (cuDoubleComplex*) x, incX, results);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_sasum(cublasHandle_t *handle, int n, float *x, int incX, float *results){

    cublasSasum(*handle, n, x, incX, results);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_dasum(cublasHandle_t *handle, int n, double *x, int incX, double *results){

    cublasDasum(*handle, n, x, incX, results);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_scasum(cublasHandle_t *handle, int n, complex float *x, int incX, float *results){

    cublasScasum(*handle, n, (cuFloatComplex*) x, incX, results);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_dzasum(cublasHandle_t *handle, int n, complex double *x, int incX, double *results){

    cublasDzasum(*handle, n, (cuDoubleComplex*) x, incX, results);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_scopy(cublasHandle_t *handle, int n, float *x, int incX, float *y, int incY){

    cublasScopy(*handle, n, x, incX, y, incY);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_dcopy(cublasHandle_t *handle, int n, double *x, int incX, double *y, int incY){

    cublasDcopy(*handle, n, x, incX, y, incY);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_ccopy(cublasHandle_t *handle, int n, complex float *x, int incX, complex float *y, int incY){

    cublasCcopy(*handle, n, (cuFloatComplex*) x, incX, (cuFloatComplex*) y, incY);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_zcopy(cublasHandle_t *handle, int n, complex double *x, int incX, complex double *y, int incY){

    cublasZcopy(*handle, n, (cuDoubleComplex*) x, incX, (cuDoubleComplex*) y, incY);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_sdot(cublasHandle_t *handle, int n, float *x, int incX, float *y, int incY, float *result){

    cublasSdot(*handle, n, x, incX, y, incY, result);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_ddot(cublasHandle_t *handle, int n, double *x, int incX, double *y, int incY, double *result){

    cublasDdot(*handle, n, x, incX, y, incY, result);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_cdotu(cublasHandle_t *handle, int n, complex float *x, int incX, complex float *y, int incY, complex float *result){

    cublasCdotu(*handle, n, (cuFloatComplex*) x, incX, (cuFloatComplex*) y, incY, (cuFloatComplex*) result);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_zdotu(cublasHandle_t *handle, int n, complex double *x, int incX, complex double *y, int incY, complex double *result){

    cublasZdotu(*handle, n, (cuDoubleComplex*) x, incX, (cuDoubleComplex*) y, incY, (cuDoubleComplex*) result);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_cdotc(cublasHandle_t *handle, int n, complex float *x, int incX, complex float *y, int incY, complex float *result){

    cublasCdotu(*handle, n, (cuFloatComplex*) x, incX, (cuFloatComplex*) y, incY, (cuFloatComplex*) result);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_zdotc(cublasHandle_t *handle, int n, complex double *x, int incX, complex double *y, int incY, complex double *result){

    cublasZdotu(*handle, n, (cuDoubleComplex*) x, incX, (cuDoubleComplex*) y, incY, (cuDoubleComplex*) result);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_snrm2(cublasHandle_t *handle, int n, float *x, int incX, float *results){

    cublasSnrm2(*handle, n, x, incX, results);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_dnrm2(cublasHandle_t *handle, int n, double *x, int incX, double *results){

    cublasDnrm2(*handle, n, x, incX, results);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_scnrm2(cublasHandle_t *handle, int n, complex float *x, int incX, float *results){

    cublasScnrm2(*handle, n, (cuFloatComplex*) x, incX, results);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_dznrm2(cublasHandle_t *handle, int n, complex double *x, int incX, double *results){

    cublasDznrm2(*handle, n, (cuDoubleComplex*) x, incX, results);
    cudaDeviceSynchronize();
    return 0;

}

int cublas_srot(cublasHandle_t *handle, int n, float *x, int incX, float *y, int incY, float c, float s){
    cublasSrot(*handle, n, x, incX, y, incY, &c, &s);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_drot(cublasHandle_t *handle, int n, double *x, int incX, double *y, int incY, double c, double s){
    cublasDrot(*handle, n, x, incX, y, incY, &c, &s);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_crot(cublasHandle_t *handle, int n, complex float *x, int incX, complex float *y, int incY, float c, complex float s){
    cublasCrot(*handle, n, (cuFloatComplex*) x, incX, (cuFloatComplex*) y, incY, &c, (cuFloatComplex*) &s);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_csrot(cublasHandle_t *handle, int n, complex float *x, int incX, complex float *y, int incY, float c, float s){
    cublasCsrot(*handle, n, (cuFloatComplex*) x, incX, (cuFloatComplex*) y, incY, &c, &s);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_zrot(cublasHandle_t *handle, int n, complex double *x, int incX, complex double *y, int incY, double c, complex double s){
    cublasZrot(*handle, n, (cuDoubleComplex*) x, incX, (cuDoubleComplex*) y, incY, &c, (cuDoubleComplex*) &s);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_zdrot(cublasHandle_t *handle, int n, complex double *x, int incX, complex double *y, int incY, double c, double s){
    cublasZdrot(*handle, n, (cuDoubleComplex*) x, incX, (cuDoubleComplex*) y, incY, &c, &s);
    cudaDeviceSynchronize();
    return 0;
}


int cublas_srotg(cublasHandle_t *handle, float a, float b, float *c, float *s){
    cublasSrotg(*handle, &a, &b, c, s);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_drotg(cublasHandle_t *handle, double a, double b, double *c, double *s){
    cublasDrotg(*handle, &a, &b, c, s);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_crotg(cublasHandle_t *handle, complex float a, complex float b, float *c, complex float *s){
    cublasCrotg(*handle, (cuFloatComplex*) &a, (cuFloatComplex*) &b, c, (cuFloatComplex*) s);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_zrotg(cublasHandle_t *handle, complex double a, complex double b, double *c, complex double *s){
    cublasZrotg(*handle, (cuDoubleComplex*) &a, (cuDoubleComplex*) &b, c, (cuDoubleComplex*) s);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_srotm(cublasHandle_t *handle, int n, float *x, int incX, float *y, int incY, float *param){
    float param_host[5];

    int i;
    for(i = 0; i < 5; i ++){
      param_host[i] = param[i];
    }

    cublasSrotm(*handle, n, x, incX, y, incY, param_host);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_drotm(cublasHandle_t *handle, int n, double *x, int incX, double *y, int incY, double *param){
    double param_host[5];

    int i;
    for(i = 0; i < 5; i ++){
      param_host[i] = param[i];
    }

    cublasDrotm(*handle, n, x, incX, y, incY, param_host);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_srotmg(cublasHandle_t *handle, float *d1, float *d2, float *x1, float *y1, float *param){
    float param_host[5];

    int i;
    for(i = 0; i < 5; i ++){
      param_host[i] = param[i];
    }

    cublasSrotmg(*handle, d1, d2, x1, y1, param_host);

    cudaDeviceSynchronize();
    for(i = 0; i < 5; i ++){
      param[i] = param_host[i];
    }

    return 0;

}

int cublas_drotmg(cublasHandle_t *handle, double *d1, double *d2, double *x1, double *y1, double *param){
    double param_host[5];

    int i;
    for(i = 0; i < 5; i ++){
      param_host[i] = param[i];
    }

    cublasDrotmg(*handle, d1, d2, x1, y1, param_host);

    cudaDeviceSynchronize();
    for(i = 0; i < 5; i ++){
      param[i] = param_host[i];
    }

    return 0;
}

int cublas_sscal(cublasHandle_t *handle, int n, float alpha, float *x, int incX){
    cublasSscal(*handle, n, &alpha, x, incX);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_dscal(cublasHandle_t *handle, int n, double alpha, double *x, int incX){
    cublasDscal(*handle, n, &alpha, x, incX);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_cscal(cublasHandle_t *handle, int n, complex float alpha, complex float *x, int incX){
    cublasCscal(*handle, n, (cuFloatComplex*) &alpha, (cuFloatComplex*) x, incX);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_zscal(cublasHandle_t *handle, int n, complex double alpha, complex double *x, int incX){
    cublasZscal(*handle, n, (cuDoubleComplex*) &alpha, (cuDoubleComplex*) x, incX);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_csscal(cublasHandle_t *handle, int n, float alpha, complex float *x, int incX){
    cublasCsscal(*handle, n, &alpha, (cuFloatComplex*) x, incX);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_zdscal(cublasHandle_t *handle, int n, double alpha, complex double *x, int incX){
    cublasZdscal(*handle, n, &alpha, (cuDoubleComplex*) x, incX);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_sswap(cublasHandle_t *handle, int n, float *x, int incX, float *y, int incY){
    cublasSswap(*handle, n, x, incX, y, incY);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_dswap(cublasHandle_t *handle, int n, double *x, int incX, double *y, int incY){
    cublasDswap(*handle, n, x, incX, y, incY);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_cswap(cublasHandle_t *handle, int n, complex float *x, int incX, complex float *y, int incY){
    cublasCswap(*handle, n, (cuFloatComplex*) x, incX, (cuFloatComplex*) y, incY);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_zswap(cublasHandle_t *handle, int n, complex double *x, int incX, complex double *y, int incY){
    cublasZswap(*handle, n, (cuDoubleComplex*) x, incX, (cuDoubleComplex*) y, incY);
    cudaDeviceSynchronize();
    return 0;
}

/*
int cublas_sgbmv(cublasHandle_t *handle, int trans, int m, int n, int kl, int ku, float alpha, float *A, int lda, float *x, int incX, float beta, float *y, int incY){
    cublasSgbmv(*handle, trans, m, n, kl, ku, &alpha, A, lda, x, incX, &beta, y, incY);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_dgbmv(cublasHandle_t *handle, int trans, int m, int n, int kl, int ku, double alpha, double *A, int lda, double *x, int incX, double beta, double *y, int incY){
    cublasDgbmv(*handle, trans, m, n, kl, ku, &alpha, A, lda, x, incX, &beta, y, incY);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_cgbmv(cublasHandle_t *handle, int trans, int m, int n, int kl, int ku, complex float alpha, complex float *A, int lda, complex float *x, int incX, complex float beta, complex float *y, int incY){
    cublasCgbmv(*handle, trans, m, n, kl, ku, (cuFloatComplex*) &alpha, (cuFloatComplex*) A, lda, (cuFloatComplex*) x, incX, (cuFloatComplex*) &beta, (cuFloatComplex*) y, incY);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_zgbmv(cublasHandle_t *handle, int trans, int m, int n, int kl, int ku, complex double alpha, complex double *A, int lda, complex double *x, int incX, complex double beta, complex double *y, int incY){
    cublasZgbmv(*handle, trans, m, n, kl, ku, (cuDoubleComplex*) &alpha, (cuDoubleComplex*) A, lda, (cuDoubleComplex*) x, incX, (cuDoubleComplex*) &beta, (cuDoubleComplex*) y, incY);
    cudaDeviceSynchronize();
    return 0;
}
*/

int cublas_sgemv(cublasHandle_t *handle, int trans, int m, int n, float alpha, float *A, int lda, float *x, int incX, float beta, float *y, int incY){
    cublasSgemv(*handle, trans, m, n, &alpha, A, lda, x, incX, &beta, y, incY);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_dgemv(cublasHandle_t *handle, int trans, int m, int n, double alpha, double *A, int lda, double *x, int incX, double beta, double *y, int incY){
    cublasDgemv(*handle, trans, m, n, &alpha, A, lda, x, incX, &beta, y, incY);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_cgemv(cublasHandle_t *handle, int trans, int m, int n, complex float alpha, complex float *A, int lda, complex float *x, int incX, complex float beta, complex float *y, int incY){
    cublasCgemv(*handle, trans, m, n, (cuFloatComplex*) &alpha, (cuFloatComplex*) A, lda, (cuFloatComplex*) x, incX, (cuFloatComplex*) &beta, (cuFloatComplex*) y, incY);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_zgemv(cublasHandle_t *handle, int trans, int m, int n, complex double alpha, complex double *A, int lda, complex double *x, int incX, complex double beta, complex double *y, int incY){
    cublasZgemv(*handle, trans, m, n, (cuDoubleComplex*) &alpha, (cuDoubleComplex*) A, lda, (cuDoubleComplex*) x, incX, (cuDoubleComplex*) &beta, (cuDoubleComplex*) y, incY);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_sger(cublasHandle_t *handle, int m, int n, float alpha, float *x, int incX, float *y, int incY, float *A, int lda){
    cublasSger(*handle, m, n, &alpha, x, incX, y, incY, A, lda);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_dger(cublasHandle_t *handle, int m, int n, double alpha, double *x, int incX, double *y, int incY, double *A, int lda){
    cublasDger(*handle, m, n, &alpha, x, incX, y, incY, A, lda);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_cgeru(cublasHandle_t *handle, int m, int n, complex float alpha, complex float *x, int incX, complex float *y, int incY, complex float *A, int lda){
    cublasCgeru(*handle, m, n, (cuFloatComplex*) &alpha, (cuFloatComplex*) x, incX, (cuFloatComplex*) y, incY, (cuFloatComplex*) A, lda);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_zgeru(cublasHandle_t *handle, int m, int n, complex double alpha, complex double *x, int incX, complex double *y, int incY, complex double *A, int lda){
    cublasZgeru(*handle, m, n, (cuDoubleComplex*) &alpha, (cuDoubleComplex*) x, incX, (cuDoubleComplex*) y, incY, (cuDoubleComplex*) A, lda);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_cgrec(cublasHandle_t *handle, int m, int n, complex float alpha, complex float *x, int incX, complex float *y, int incY, complex float *A, int lda){
    cublasCgerc(*handle, m, n, (cuFloatComplex*) &alpha, (cuFloatComplex*) x, incX, (cuFloatComplex*) y, incY, (cuFloatComplex*) A, lda);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_zgrec(cublasHandle_t *handle, int m, int n, complex double alpha, complex double *x, int incX, complex double *y, int incY, complex double *A, int lda){
    cublasZgerc(*handle, m, n, (cuDoubleComplex*) &alpha, (cuDoubleComplex*) x, incX, (cuDoubleComplex*) y, incY, (cuDoubleComplex*) A, lda);
    cudaDeviceSynchronize();
    return 0;
}

int cublas_sgemm(cublasHandle_t *handle, int transa, int transb, int m, int n, int k, float alpha, float *A, int lda, float *B, int ldb, float beta, float *C, int ldc){

    cublasSgemm(*handle, transa, transb, m, n, k, &alpha, A, lda, B, ldb, &beta, C, ldc);
    cudaDeviceSynchronize();
    return 0;

}
