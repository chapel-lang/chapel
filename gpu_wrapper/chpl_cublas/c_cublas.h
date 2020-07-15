void *cublas_create();
void cublas_destroy(c_void_ptr *handle);
float *cublas_array(size_t size);
int cublas_saxpy(c_void_ptr *handle, int N, float alpha, float *x, int incX, float *y, int incY);
