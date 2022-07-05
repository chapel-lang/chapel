
void chpl_gpu_init() {
  chpl_gpu_test();
  chpl_gpu_plugin_init_device();
}

bool chpl_gpu_is_device_ptr(void* ptr) {
  chpl_internal_error("gpu is device ptr is not implemented yet");
}

size_t chpl_gpu_get_alloc_size(void* ptr) {
  chpl_internal_error("gpu get alloc _size is not implemented yet");
}

bool chpl_gpu_has_context() {
  chpl_internal_error("gpu has context is not implemented yet");
}

void chpl_gpu_launch_kernel(int ln, int32_t fn,
                            const char* fatbinData, const char* name,
                            int grd_dim_x, int grd_dim_y, int grd_dim_z,
                            int blk_dim_x, int blk_dim_y, int blk_dim_z,
                            int nargs, ...) {
  chpl_internal_error("gpu launch kernel not implemented yet");
}

void chpl_gpu_launch_kernel_flat(int ln, int32_t fn,
                                 const char* fatbinData, const char* name,
                                 int num_threads, int blk_dim, int nargs, ...) {
  chpl_internal_error("gpu launch kernel flat not implemented yet");
}

void* chpl_gpu_mem_alloc(size_t size, chpl_mem_descInt_t description,
                         int32_t lineno, int32_t filename) {
  CUdeviceptr ptr = 0;
  ptr = (CUdeviceptr)chpl_gpu_plugin_mem_alloc(size, 0);
  assert(ptr);
}

void* chpl_gpu_mem_calloc(size_t number, size_t size,
                          chpl_mem_descInt_t description,
                          int32_t lineno, int32_t filename) {

  chpl_internal_error("gpu mem calloc not implemented yet");
}

void* chpl_gpu_mem_realloc(void* memAlloc, size_t size,
                           chpl_mem_descInt_t description,
                           int32_t lineno, int32_t filename) {
  chpl_internal_error("gpu realloc not implemented yet");
}

void* chpl_gpu_mem_memalign(size_t boundary, size_t size,
                            chpl_mem_descInt_t description,
                            int32_t lineno, int32_t filename) {
  chpl_internal_error("gpu memalign not implemented yet");
}

void chpl_gpu_mem_free(void* memAlloc, int32_t lineno, int32_t filename) {
  chpl_internal_error("gpu mem free not implemented yet");
}
