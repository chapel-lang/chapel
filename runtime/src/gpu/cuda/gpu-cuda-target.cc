#include "chplrt.h"
#include "gpu-cuda-target.h"
#include "dlfcn.h"

/*#ifdef __cplusplus*/
#include "device.h"
#include "omptargetplugin.h"
/*#endif*/

//void initialize() {
  //PM->RTLs.initAllRTLs();
  //DeviceTy &d = *PM->Devices[0];
  //printf("isinit: %d\n", d.IsInit);
//}

extern "C" {

void chpl_gpu_test() {
  //initialize();

  void *rtl_handle = dlopen("libomptarget.rtl.cuda.so", RTLD_NOW);
  assert(rtl_handle);
  int32_t (*num_devices)(void) = (int32_t(*)())dlsym(rtl_handle, "__tgt_rtl_number_of_devices");
  assert(num_devices);



  printf("In gpu tsest\n");
  printf("%d\n", num_devices());
}



}
