#include <complex.h>
#include "cublas_v2.h"

void* cublas_create(void);
void cublas_destroy(cublasHandle_t *handle);
float *cublas_array(size_t size);
void* to_gpu(cublasHandle_t *src_ptr, size_t size);
void to_cpu(void* dst_ptr, void* src_ptr, size_t size);
//int cublas_axpy(cublasHandle_t *handle, int N, float alpha, void *x, int incX, void *y, int incY);

int cublas_saxpy(cublasHandle_t *handle, int N, float alpha, float *x, int incX, float *y, int incY);
int cublas_daxpy(cublasHandle_t *handle, int N, double alpha, double *x, int incX, double *y, int incY);
int cublas_caxpy(cublasHandle_t *handle, int N, complex float alpha, complex float *x, int incX, complex float *y, int incY);
int cublas_zaxpy(cublasHandle_t *handle, int N, complex double alpha, complex double *x, int incX, complex double *y, int incY);

int cublas_isamax(cublasHandle_t *handle, int n, float *x, int incX, int *results);
int cublas_idamax(cublasHandle_t *handle, int n, double *x, int incX, int *results);
int cublas_icamax(cublasHandle_t *handle, int n, complex float *x, int incX, int *results);
int cublas_izamax(cublasHandle_t *handle, int n, complex double *x, int incX, int *results);

int cublas_isamin(cublasHandle_t *handle, int n, float *x, int incX, int *results);
int cublas_idamin(cublasHandle_t *handle, int n, double *x, int incX, int *results);
int cublas_icamin(cublasHandle_t *handle, int n, complex float *x, int incX, int *results);
int cublas_izamin(cublasHandle_t *handle, int n, complex double *x, int incX, int *results);

int cublas_sasum(cublasHandle_t *handle, int n, float *x, int incX, float *results);
int cublas_dasum(cublasHandle_t *handle, int n, double *x, int incX, double *results);
int cublas_scasum(cublasHandle_t *handle, int n, complex float *x, int incX, float *results);
int cublas_dzasum(cublasHandle_t *handle, int n, complex double *x, int incX, double *results);

int cublas_scopy(cublasHandle_t *handle, int n, float *x, int incX, float *y, int incY);
int cublas_dcopy(cublasHandle_t *handle, int n, double *x, int incX, double *y, int incY);
int cublas_ccopy(cublasHandle_t *handle, int n, complex float *x, int incX, complex float *y, int incY);
int cublas_zcopy(cublasHandle_t *handle, int n, complex double *x, int incX, complex double *y, int incY);

int cublas_sdot(cublasHandle_t *handle, int n, float *x, int incX, float *y, int incY, float *result);
int cublas_ddot(cublasHandle_t *handle, int n, double *x, int incX, double *y, int incY, double *result);
int cublas_cdotu(cublasHandle_t *handle, int n, complex float *x, int incX, complex float *y, int incY, complex float *result);
int cublas_zdotu(cublasHandle_t *handle, int n, complex double *x, int incX, complex double *y, int incY, complex double *result);
int cublas_cdotc(cublasHandle_t *handle, int n, complex float *x, int incX, complex float *y, int incY, complex float *result);
int cublas_zdotc(cublasHandle_t *handle, int n, complex double *x, int incX, complex double *y, int incY, complex double *result);

int cublas_snrm2(cublasHandle_t *handle, int n, float *x, int incX, float *results);
int cublas_dnrm2(cublasHandle_t *handle, int n, double *x, int incX, double *results);
int cublas_scnrm2(cublasHandle_t *handle, int n, complex float *x, int incX, float *results);
int cublas_dznrm2(cublasHandle_t *handle, int n, complex double *x, int incX, double *results);

int cublas_srot(cublasHandle_t *handle, int n, float *x, int incX, float *y, int incY, float c, float s);
int cublas_drot(cublasHandle_t *handle, int n, double *x, int incX, double *y, int incY, double c, double s);
int cublas_crot(cublasHandle_t *handle, int n, complex float *x, int incX, complex float *y, int incY, float c, complex float s);
int cublas_csrot(cublasHandle_t *handle, int n, complex float *x, int incX, complex float *y, int incY, float c, float s);
int cublas_zrot(cublasHandle_t *handle, int n, complex double *x, int incX, complex double *y, int incY,  double c, complex double s);
int cublas_zdrot(cublasHandle_t *handle, int n, complex double *x, int incX, complex double *y, int incY,  double c, double s);

int cublas_srotg(cublasHandle_t *handle, float a, float b, float *c, float *s);
int cublas_drotg(cublasHandle_t *handle, double a, double b, double *c, double *s);
int cublas_crotg(cublasHandle_t *handle, complex float a, complex float b, float *c, complex float *s);
int cublas_zrotg(cublasHandle_t *handle, complex double a, complex double b, double *c, complex double *s);

int cublas_srotm(cublasHandle_t *handle, int n, float *x, int incX, float *y, int incY, float *param);
int cublas_drotm(cublasHandle_t *handle, int n, double *x, int incX, double *y, int incY, double *param);

int cublas_srotmg(cublasHandle_t *handle, float *d1, float *d2, float *x1, float *y1, float *param);
int cublas_drotmg(cublasHandle_t *handle, double *d1, double *d2, double  *x1, double  *y1, double *param);

int cublas_sscal(cublasHandle_t *handle, int n, float alpha, float *x, int incX);
int cublas_dscal(cublasHandle_t *handle, int n, double alpha, double *x, int incX);
int cublas_cscal(cublasHandle_t *handle, int n, complex float alpha, complex float *x, int incX);
int cublas_csscal(cublasHandle_t *handle, int n, float alpha, complex float *x, int incX);
int cublas_zscal(cublasHandle_t *handle, int n, complex double alpha, complex double *x, int incX);
int cublas_zdscal(cublasHandle_t *handle, int n, double alpha, complex double *x, int incX);

int cublas_sswap(cublasHandle_t *handle, int n, float *x, int incX, float *y, int incY);
int cublas_dswap(cublasHandle_t *handle, int n, double *x, int incX, double *y, int incY);
int cublas_cswap(cublasHandle_t *handle, int n, complex float *x, int incX, complex float *y, int incY);
int cublas_zswap(cublasHandle_t *handle, int n, complex double *x, int incX, complex double *y, int incY);
/*
int cublas_sgbmv(cublasHandle_t *handle, int trans, int m, int n, int kl, int ku, float alpha, float *A, int lda, float *x, int incX, float beta, float *y, int incY)
int cublas_dgbmv(cublasHandle_t *handle, int trans, int m, int n, int kl, int ku, double alpha, double *A, int lda, double *x, int incX, double beta, double *y, int incY)
int cublas_cgbmv(cublasHandle_t *handle, int trans, int m, int n, int kl, int ku, complex float alpha, complex float *A, int lda, complex float *x, int incX, complex float beta, complex float *y, int incY)
int cublas_zgbmv(cublasHandle_t *handle, int trans, int m, int n, int kl, int ku, complex double alpha, complex double *A, int lda, complex double *x, int incX, complex double beta, complex double *y, int incY)
*/

int cublas_sgemv(cublasHandle_t *handle, int trans, int m, int n, float alpha, float *A, int lda, float *x, int incX, float beta, float *y, int incY);
int cublas_dgemv(cublasHandle_t *handle, int trans, int m, int n, double alpha, double *A, int lda, double *x, int incX, double beta, double *y, int incY);
int cublas_cgemv(cublasHandle_t *handle, int trans, int m, int n, complex float alpha, complex float *A, int lda, complex float *x, int incX, complex float beta, complex float *y, int incY);
int cublas_zgemv(cublasHandle_t *handle, int trans, int m, int n, complex double alpha, complex double *A, int lda, complex double *x, int incX, complex double beta, complex double *y, int incY);

int cublas_sger(cublasHandle_t *handle, int m, int n, float alpha, float *x, int incX, float *y, int incY, float *A, int lda);
int cublas_dger(cublasHandle_t *handle, int m, int n, double alpha, double *x, int incX, double *y, int incY,double *A, int lda);
int cublas_cgeru(cublasHandle_t *handle, int m, int n, complex float alpha, complex float *x, int incX, complex float *y, int incY, complex float *A, int lda);
int cublas_cgerc(cublasHandle_t *handle, int m, int n, complex float alpha, complex float *x, int incX, complex float *y, int incY, complex float *A, int lda);
int cublas_zgeru(cublasHandle_t *handle, int m, int n, complex double alpha, complex double *x, int incX, complex double *y, int incY, complex double *A, int lda);
int cublas_zgerc(cublasHandle_t *handle, int m, int n, complex double alpha, complex double *x, int incX, complex double *y, int incY, complex double *A, int lda);


int cublas_sgemm(cublasHandle_t *handle, int transa, int transb, int m, int n, int k, float alpha, float *A, int lda, float *B, int ldb, float beta, float *C, int ldc);
