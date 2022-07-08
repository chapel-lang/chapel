
#ifdef __cplusplus
extern "C" {
#endif

void chpl_gpu_impl_init(void);

void chpl_gpu_impl_launch_kernel(int ln, int32_t fn,
                                 const char* fatbinData, const char* name,
                                 int grd_dim_x, int grd_dim_y, int grd_dim_z,
                                 int blk_dim_x, int blk_dim_y, int blk_dim_z,
                                 int nargs, va_list args);
void chpl_gpu_impl_launch_kernel_flat(int ln, int32_t fn,
                                 const char* fatbinPath, const char* name,
                                 int num_threads, int blk_dim,
                                 int nargs, va_list args);

void* chpl_gpu_impl_mem_alloc(size_t size);
void chpl_gpu_impl_mem_free(void* memAlloc);

void chpl_gpu_impl_copy_device_to_host(void* dst, void* src, size_t n);
void chpl_gpu_impl_copy_host_to_device(void* dst, void* src, size_t n);

// this is all about copying within the same device that is on this subloc
void chpl_gpu_impl_copy_device_to_device(void* dst, void* src, size_t n);

// module code uses this to pick the right deallocator for a pointer
bool chpl_gpu_impl_is_device_ptr(void* ptr);

// TODO do we really need to expose this?
size_t chpl_gpu_impl_get_alloc_size(void* ptr);

#ifdef __cplusplus
}
#endif
