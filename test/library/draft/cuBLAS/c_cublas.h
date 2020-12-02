void* cublas_create(void);
void cublas_destroy(c_void_ptr *handle);
float *cublas_array(size_t size);
void* to_gpu(c_void_ptr *src_ptr, size_t size);
void to_cpu(void* dst_ptr, void* src_ptr, size_t size);
//int cublas_axpy(c_void_ptr *handle, int N, float alpha, void *x, int incX, void *y, int incY);

int cublas_saxpy(c_void_ptr *handle, int N, float alpha, float *x, int incX, float *y, int incY);
int cublas_daxpy(c_void_ptr *handle, int N, double alpha, double *x, int incX, double *y, int incY);
int cublas_caxpy(c_void_ptr *handle, int N, complex float alpha, complex float *x, int incX, complex float *y, int incY);
int cublas_zaxpy(c_void_ptr *handle, int N, complex double alpha, complex double *x, int incX, complex double *y, int incY);

int cublas_isamax(c_void_ptr *handle, int n, float *x, int incX, int *results);
int cublas_idamax(c_void_ptr *handle, int n, double *x, int incX, int *results);
int cublas_icamax(c_void_ptr *handle, int n, complex float *x, int incX, int *results);
int cublas_izamax(c_void_ptr *handle, int n, complex double *x, int incX, int *results);

int cublas_isamin(c_void_ptr *handle, int n, float *x, int incX, int *results);
int cublas_idamin(c_void_ptr *handle, int n, double *x, int incX, int *results);
int cublas_icamin(c_void_ptr *handle, int n, complex float *x, int incX, int *results);
int cublas_izamin(c_void_ptr *handle, int n, complex double *x, int incX, int *results);

int cublas_sasum(c_void_ptr *handle, int n, float *x, int incX, float *results);
int cublas_dasum(c_void_ptr *handle, int n, double *x, int incX, double *results);
int cublas_scasum(c_void_ptr *handle, int n, complex float *x, int incX, float *results);
int cublas_dzasum(c_void_ptr *handle, int n, complex double *x, int incX, double *results);

int cublas_scopy(c_void_ptr *handle, int n, float *x, int incX, float *y, int incY);
int cublas_dcopy(c_void_ptr *handle, int n, double *x, int incX, double *y, int incY);
int cublas_ccopy(c_void_ptr *handle, int n, complex float *x, int incX, complex float *y, int incY);
int cublas_zcopy(c_void_ptr *handle, int n, complex double *x, int incX, complex double *y, int incY);

int cublas_sdot(c_void_ptr *handle, int n, float *x, int incX, float *y, int incY, float *result);
int cublas_ddot(c_void_ptr *handle, int n, double *x, int incX, double *y, int incY, double *result);
int cublas_cdotu(c_void_ptr *handle, int n, complex float *x, int incX, complex float *y, int incY, complex float *result);
int cublas_zdotu(c_void_ptr *handle, int n, complex double *x, int incX, complex double *y, int incY, complex double *result);
int cublas_cdotc(c_void_ptr *handle, int n, complex float *x, int incX, complex float *y, int incY, complex float *result);
int cublas_zdotc(c_void_ptr *handle, int n, complex double *x, int incX, complex double *y, int incY, complex double *result);

int cublas_snrm2(c_void_ptr *handle, int n, float *x, int incX, float *results);
int cublas_dnrm2(c_void_ptr *handle, int n, double *x, int incX, double *results);
int cublas_scnrm2(c_void_ptr *handle, int n, complex float *x, int incX, float *results);
int cublas_dznrm2(c_void_ptr *handle, int n, complex double *x, int incX, double *results);

int cublas_srot(c_void_ptr *handle, int n, float *x, int incX, float *y, int incY, float c, float s);
int cublas_drot(c_void_ptr *handle, int n, double *x, int incX, double *y, int incY, double c, double s);
int cublas_crot(c_void_ptr *handle, int n, complex float *x, int incX, complex float *y, int incY, float c, complex float s);
int cublas_csrot(c_void_ptr *handle, int n, complex float *x, int incX, complex float *y, int incY, float c, float s);
int cublas_zrot(c_void_ptr *handle, int n, complex double *x, int incX, complex double *y, int incY,  double c, complex double s);
int cublas_zdrot(c_void_ptr *handle, int n, complex double *x, int incX, complex double *y, int incY,  double c, double s);

int cublas_srotg(c_void_ptr *handle, float a, float b, float *c, float *s);
int cublas_drotg(c_void_ptr *handle, double a, double b, double *c, double *s);
int cublas_crotg(c_void_ptr *handle, complex float a, complex float b, float *c, complex float *s);
int cublas_zrotg(c_void_ptr *handle, complex double a, complex double b, double *c, complex double *s);

int cublas_srotm(c_void_ptr *handle, int n, float *x, int incX, float *y, int incY, float *param);
int cublas_drotm(c_void_ptr *handle, int n, double *x, int incX, double *y, int incY, double *param);

int cublas_srotmg(c_void_ptr *handle, float *d1, float *d2, float *x1, float *y1, float *param);
int cublas_drotmg(c_void_ptr *handle, double *d1, double *d2, double  *x1, double  *y1, double *param);

int cublas_sscal(c_void_ptr *handle, int n, float alpha, float *x, int incX);
int cublas_dscal(c_void_ptr *handle, int n, double alpha, double *x, int incX);
int cublas_cscal(c_void_ptr *handle, int n, complex float alpha, complex float *x, int incX);
int cublas_csscal(c_void_ptr *handle, int n, float alpha, complex float *x, int incX);
int cublas_zscal(c_void_ptr *handle, int n, complex double alpha, complex double *x, int incX);
int cublas_zdscal(c_void_ptr *handle, int n, double alpha, complex double *x, int incX);

int cublas_sswap(c_void_ptr *handle, int n, float *x, int incX, float *y, int incY);
int cublas_dswap(c_void_ptr *handle, int n, double *x, int incX, double *y, int incY);
int cublas_cswap(c_void_ptr *handle, int n, complex float *x, int incX, complex float *y, int incY);
int cublas_zswap(c_void_ptr *handle, int n, complex double *x, int incX, complex double *y, int incY);
/*
int cublas_sgbmv(cublasHandle_t *handle, int trans, int m, int n, int kl, int ku, float alpha, float *A, int lda, float *x, int incX, float beta, float *y, int incY)
int cublas_dgbmv(cublasHandle_t *handle, int trans, int m, int n, int kl, int ku, double alpha, double *A, int lda, double *x, int incX, double beta, double *y, int incY)
int cublas_cgbmv(cublasHandle_t *handle, int trans, int m, int n, int kl, int ku, complex float alpha, complex float *A, int lda, complex float *x, int incX, complex float beta, complex float *y, int incY)
int cublas_zgbmv(cublasHandle_t *handle, int trans, int m, int n, int kl, int ku, complex double alpha, complex double *A, int lda, complex double *x, int incX, complex double beta, complex double *y, int incY)
*/

int cublas_sgemv(c_void_ptr *handle, int trans, int m, int n, float alpha, float *A, int lda, float *x, int incX, float beta, float *y, int incY);
int cublas_dgemv(c_void_ptr *handle, int trans, int m, int n, double alpha, double *A, int lda, double *x, int incX, double beta, double *y, int incY);
int cublas_cgemv(c_void_ptr *handle, int trans, int m, int n, complex float alpha, complex float *A, int lda, complex float *x, int incX, complex float beta, complex float *y, int incY);
int cublas_zgemv(c_void_ptr *handle, int trans, int m, int n, complex double alpha, complex double *A, int lda, complex double *x, int incX, complex double beta, complex double *y, int incY);

int cublas_sger(c_void_ptr *handle, int m, int n, float alpha, float *x, int incX, float *y, int incY, float *A, int lda);
int cublas_dger(c_void_ptr *handle, int m, int n, double alpha, double *x, int incX, double *y, int incY,double *A, int lda);
int cublas_cgeru(c_void_ptr *handle, int m, int n, complex float alpha, complex float *x, int incX, complex float *y, int incY, complex float *A, int lda);
int cublas_cgerc(c_void_ptr *handle, int m, int n, complex float alpha, complex float *x, int incX, complex float *y, int incY, complex float *A, int lda);
int cublas_zgeru(c_void_ptr *handle, int m, int n, complex double alpha, complex double *x, int incX, complex double *y, int incY, complex double *A, int lda);
int cublas_zgerc(c_void_ptr *handle, int m, int n, complex double alpha, complex double *x, int incX, complex double *y, int incY, complex double *A, int lda);


int cublas_sgemm(c_void_ptr *handle, int transa, int transb, int m, int n, int k, float alpha, float *A, int lda, float *B, int ldb, float beta, float *C, int ldc);
