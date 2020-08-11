void* cublas_create(void);
void cublas_destroy(c_void_ptr *handle);
float *cublas_array(size_t size);
void* to_gpu(c_void_ptr *src_ptr, size_t size);
void to_cpu(void* dst_ptr, void* src_ptr, size_t size);
//int cublas_axpy(c_void_ptr *handle, int N, float alpha, void *x, int incX, void *y, int incY);

int cublas_saxpy(c_void_ptr *handle, int N, float alpha, float *x, int incX, float *y, int incY);
int cublas_daxpy(c_void_ptr *handle, int N, double alpha, double *x, int incX, double *y, int incY);
int cublas_caxpy(c_void_ptr *handle, int N, complex alpha, complex *x, int incX, complex *y, int incY);

int cublas_isamax(c_void_ptr *handle, int n, float *x, int incX, int *results);
int cublas_idamax(c_void_ptr *handle, int n, double *x, int incX, int *results);

int cublas_isamin(c_void_ptr *handle, int n, float *x, int incX, int *results);
int cublas_idamin(c_void_ptr *handle, int n, double *x, int incX, int *results);

int cublas_sasum(c_void_ptr *handle, int n, float *x, int incX, float *results);
int cublas_dasum(c_void_ptr *handle, int n, double *x, int incX, double *results);

int cublas_scopy(c_void_ptr *handle, int n, float *x, int incX, float *y, int incY);
int cublas_dcopy(c_void_ptr *handle, int n, double *x, int incX, double *y, int incY);

int cublas_sdot(c_void_ptr *handle, int n, float *x, int incX, float *y, int incY, float *result);
int cublas_ddot(c_void_ptr *handle, int n, double *x, int incX, double *y, int incY, double *result);

int cublas_snrm2(c_void_ptr *handle, int n, float *x, int incX, float *results);
int cublas_dnrm2(c_void_ptr *handle, int n, double *x, int incX, double *results);

int cublas_srot(c_void_ptr *handle, int n, float *x, int incX, float *y, int incY, float c, float s);
int cublas_drot(c_void_ptr *handle, int n, double *x, int incX, double *y, int incY, double c, double s);

int cublas_srotg(c_void_ptr *handle, float a, float b, float c, float s);
int cublas_drotg(c_void_ptr *handle, double a, double b, double c, double s);

int cublas_srotm(c_void_ptr *handle, int n, float *x, int incX, float *y, int incY, float *param);
int cublas_drotm(c_void_ptr *handle, int n, double *x, int incX, double *y, int incY, double *param);

int cublas_srotmg(c_void_ptr *handle, float d1, float d2, float x1, float y1, float *param);
int cublas_drotmg(c_void_ptr *handle, double d1, double d2, double  x1, double  y1, float *param);

int cublas_sscal(c_void_ptr *handle, int n, float alpha, float *x, int incX);
int cublas_dscal(c_void_ptr *handle, int n, double alpha, double *x, int incX);

int cublas_sswap(c_void_ptr *handle, int n, float *x, int incX, float *y, int incY);
int cublas_dswap(c_void_ptr *handle, int n, double *x, int incX, double *y, int incY);

int cublas_sgemm(c_void_ptr *handle, int transa, int transb, int m, int n, int k, float alpha, float *A, int lda, float *B, int ldb, float beta, float *C, int ldc);
