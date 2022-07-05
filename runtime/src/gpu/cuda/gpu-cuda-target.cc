#include "chplrt.h"
#include "gpu-cuda-target.h"
#include "dlfcn.h"
#include "chpl-mem.h"
#include "chpl-tasks.h"

/*#ifdef __cplusplus*/
/*#include "device.h"*/
#include "omptargetplugin.h"
/*#endif*/

typedef int32_t(init_device_fn)(int32_t);
typedef int32_t(num_devices_fn)(void);
typedef void *(mem_alloc_fn)(int32_t, int64_t, void*, int32_t);


typedef struct chpl_gpu_plugin_rtl_s {
  void *handle = NULL;
  init_device_fn *init_device = NULL;
  num_devices_fn *num_devices = NULL;
  mem_alloc_fn *mem_alloc = NULL;

} chpl_gpu_plugin_rtl_t;

static chpl_gpu_plugin_rtl_t *rtl = NULL;

extern "C" {

#define ALLOC_FN_VAR(name) void *(*name)(int32_t, int64_t, void*, int32_t)
#define ALLOC_FN_TYPE ALLOC_FN_VAR()

void chpl_gpu_test() {
  //initialize();

  //void *rtl_handle = dlopen("libomptarget.rtl.cuda.so", RTLD_NOW);
  //assert(rtl_handle);
  //int32_t (*num_devices)(void) = (int32_t(*)())dlsym(rtl_handle, "__tgt_rtl_number_of_devices");
  //assert(num_devices);



  printf("In gpu tsest\n");
}

void chpl_gpu_plugin_init_device(void) {
  if (rtl == NULL) {
    rtl = (chpl_gpu_plugin_rtl_t*)chpl_malloc(sizeof(chpl_gpu_plugin_rtl_t));

    rtl->handle = dlopen("/home/engin/code/chapel/versions/1/chapel/third-party/llvm/install/linux64-x86_64/lib/libomptarget.rtl.cuda.so", RTLD_NOW);
    assert(rtl->handle);

    rtl->num_devices = (num_devices_fn*)dlsym(rtl->handle, "__tgt_rtl_number_of_devices");
    assert(rtl->num_devices);
    printf("%d\n", (*(rtl->num_devices))());

    printf("100\n");
    rtl->mem_alloc = (mem_alloc_fn*)dlsym(rtl->handle, "__tgt_rtl_data_alloc");
    assert(rtl->mem_alloc);
    printf("110\n");

    rtl->init_device = (init_device_fn*)dlsym(rtl->handle, "__tgt_rtl_init_device");
    assert(rtl->init_device);
    printf("120\n");
    //int32_t subloc = chpl_task_getRequestedSubloc();
    (*(rtl->init_device))(0);
    printf("200\n");
  }
}

void* chpl_gpu_plugin_mem_alloc(size_t size, int32_t device_id) {

  //void *rtl_handle = dlopen("libomptarget.rtl.cuda.so", RTLD_NOW);
  //assert(rtl_handle);
  //ALLOC_FN_VAR(alloc) = (ALLOC_FN_TYPE)dlsym(rtl_handle, "__tgt_rtl_data_alloc");
  //assert(alloc);

  printf("Allocating using the plugin\n");
  printf("%zu %d\n", size, device_id);
  printf("%p\n", rtl->mem_alloc);
  void* res = (*(rtl->mem_alloc))(device_id, size, NULL, TARGET_ALLOC_SHARED);
  assert(res);
  return res;
}



}
