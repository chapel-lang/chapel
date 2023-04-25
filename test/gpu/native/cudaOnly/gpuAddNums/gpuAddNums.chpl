// See the README file for more information about this test.

extern {
  #include <cuda.h>
  #include <assert.h>
  #include <stdio.h>
  #include <stdbool.h>

  #define FATBIN_FILE "gpuAddNums_gpu_files/chpl__gpu.fatbin"

  static void checkCudaErrors(CUresult err, int i) {
    if (err != CUDA_SUCCESS) {
      const char* name;
      cuGetErrorName(err, &name);
      printf("Error in call %d code:%d error name:%s\n", i, err, name);
      fflush(stdout);
      assert(false);
    }
  }

  extern char* chpl_gpuBinary;

  static double launchKernel(){
    CUdevice    device;
    CUmodule    cudaModule;
    CUfunction  function;
    int         devCount;
    CUcontext   context = NULL;;

    double X;

    cuCtxGetCurrent(&context);

    if (context == NULL) {
      checkCudaErrors(cuInit(0), 1);
      checkCudaErrors(cuDeviceGetCount(&devCount), 2);
      checkCudaErrors(cuDeviceGet(&device, 0), 3);

      char name[128];
      checkCudaErrors(cuDeviceGetName(name, 128, device), 4);

      checkCudaErrors(cuCtxCreate(&context, CU_CTX_BLOCKING_SYNC, device), 5);
    }

    checkCudaErrors(cuModuleLoadData(&cudaModule, chpl_gpuBinary), 6);

    checkCudaErrors(cuModuleGetFunction(&function, cudaModule, "add_nums"), 7);

    CUdeviceptr devBufferX;

    checkCudaErrors(cuMemAlloc(&devBufferX, sizeof(double)), 8);

    srand(0);
    X = rand() % 100;

    checkCudaErrors(cuMemcpyHtoD(devBufferX, &X, sizeof(double)), 9);

    unsigned blockSizeX = 1;
    unsigned blockSizeY = 1;
    unsigned blockSizeZ = 1;
    unsigned gridSizeX  = 1;
    unsigned gridSizeY  = 1;
    unsigned gridSizeZ  = 1;

    void *KernelParams[] = { &devBufferX };

    checkCudaErrors(cuLaunchKernel(function, gridSizeX, gridSizeY, gridSizeZ,
                                   blockSizeX, blockSizeY, blockSizeZ,
                                   0, NULL, KernelParams, NULL), 10);

    checkCudaErrors(cuMemcpyDtoH(&X, devBufferX, sizeof(double)), 11);

    return X;

  }

}

pragma "codegen for GPU"
pragma "always resolve function"
export proc add_nums(dst_ptr: c_ptr(real(64))){
  //var a = 1+2;
  dst_ptr[0] = dst_ptr[0]+10;
}

var output: real(64);
output = launchKernel();
writeln(output);
