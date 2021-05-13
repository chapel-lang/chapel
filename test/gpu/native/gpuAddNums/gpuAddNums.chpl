extern {
  #include <cuda.h>
  #include <assert.h>

  #define FATBIN_FILE "tmp/chpl__gpu.fatbin"

  static void checkCudaErrors(CUresult err) {
    assert(err == CUDA_SUCCESS);
  }

  static double launchKernel(){
    CUdevice    device;
    CUmodule    cudaModule;
    CUcontext   context;
    CUfunction  function;
    int         devCount;

    double X;

    checkCudaErrors(cuInit(0));
    checkCudaErrors(cuDeviceGetCount(&devCount));
    checkCudaErrors(cuDeviceGet(&device, 0));

    char name[128];
    checkCudaErrors(cuDeviceGetName(name, 128, device));

    checkCudaErrors(cuCtxCreate(&context, 0, device));

    char * buffer = 0;
    long length;
    FILE * f = fopen (FATBIN_FILE, "rb");

    if (f)
    {
      fseek (f, 0, SEEK_END);
      length = ftell (f);
      fseek (f, 0, SEEK_SET);
      buffer = (char* )malloc (length);
      if (buffer)
      {
        fread (buffer, 1, length, f);
      }
      fclose (f);
    }


    checkCudaErrors(cuModuleLoadData(&cudaModule, buffer));

    checkCudaErrors(cuModuleGetFunction(&function, cudaModule, "add_nums"));

    CUdeviceptr devBufferX;

    checkCudaErrors(cuMemAlloc(&devBufferX, sizeof(double)));

    srand(0);
    X = rand() % 100;

    checkCudaErrors(cuMemcpyHtoD(devBufferX, &X, sizeof(double)));

    unsigned blockSizeX = 1;
    unsigned blockSizeY = 1;
    unsigned blockSizeZ = 1;
    unsigned gridSizeX  = 1;
    unsigned gridSizeY  = 1;
    unsigned gridSizeZ  = 1;

    void *KernelParams[] = { &devBufferX };

    checkCudaErrors(cuLaunchKernel(function, gridSizeX, gridSizeY, gridSizeZ,
                                   blockSizeX, blockSizeY, blockSizeZ,
                                   0, NULL, KernelParams, NULL));

    checkCudaErrors(cuMemcpyDtoH(&X, devBufferX, sizeof(double)));

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
