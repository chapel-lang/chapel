void *cublas_create();
void cublas_destroy(c_void_ptr *handle);
float *cublas_array(size_t size);
int cublas_saxpy(c_void_ptr *handle, int N, float alpha, float *x, int incX, float *y, int incY);
int cublas_sgemm(c_void_ptr *handle, int transa, int transb, int m, int n, int k, float alpha, float *A, int lda, float *B, int ldb, float beta, float *C, int ldc);

