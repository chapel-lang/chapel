void *cublas_create();
void cublas_destroy(c_void_ptr *handle);
float *cublas_array(size_t size);
float* to_gpu(float* src_ptr, size_t size);
void to_cpu(void* dst_ptr, void* src_ptr, size_t size);
int cublas_axpy(c_void_ptr *handle, int N, float alpha, void *x, int incX, void *y, int incY);
int cublas_saxpy(c_void_ptr *handle, int N, float alpha, float *x, int incX, float *y, int incY);
int cublas_sgemm(c_void_ptr *handle, int transa, int transb, int m, int n, int k, float alpha, float *A, int lda, float *B, int ldb, float beta, float *C, int ldc);

