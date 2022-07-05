
#ifdef __cplusplus
extern "C" {
#endif

void chpl_gpu_test(void);

void chpl_gpu_plugin_init_device(void);
void* chpl_gpu_plugin_mem_alloc(size_t size, int32_t device_id);


#ifdef __cplusplus
}
#endif
