extern {
  #include <cuda.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <assert.h>

  #define FATBIN_FILE "tmp/chpl__gpu.fatbin"

  static void checkCudaErrors(CUresult err) {
    assert(err == CUDA_SUCCESS);
  }

  static void initializeCuda(){
    CUdevice    device;
    CUcontext   context;
    int         devCount;

    // CUDA initialization
    cuInit(0);

    cuDeviceGetCount(&devCount);

    cuDeviceGet(&device, 0);

    // Create driver context
    checkCudaErrors(cuCtxCreate(&context, 0, device));
  }

  static void* createFunction(){
    CUmodule    cudaModule;
    CUfunction  function;

    //read in fatbin and store in buffer
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


    // Create module for object
    cuModuleLoadData(&cudaModule, buffer);


    // Get kernel function
    cuModuleGetFunction(&function, cudaModule, "add_nums");

    return (void*)function;
  }

  static CUdeviceptr getDeviceBufferPointer(){

    double X;
    CUdeviceptr devBufferX;

    checkCudaErrors(cuMemAlloc(&devBufferX, sizeof(double)));

    srand(0);
    X = rand() % 100;
    
    checkCudaErrors(cuMemcpyHtoD(devBufferX, &X, sizeof(double)));

    return devBufferX;

  }

  static void **getKernelParams(CUdeviceptr *devBufferX){
    static void* kernelParams[1];
    kernelParams[0] = devBufferX;
    return kernelParams;
  }


  static double getDataFromDevice(CUdeviceptr devBufferX){
    double X;
    cuMemcpyDtoH(&X, devBufferX, sizeof(double));
    return X;
  }

}

pragma "codegen for GPU"
pragma "always resolve function"
export proc add_nums(dst_ptr: c_ptr(real(64))){
  dst_ptr[0] = dst_ptr[0]+5;
}


proc main() {
var output: real(64);
initializeCuda();
var funcPtr: c_void_ptr = createFunction();
var deviceBuffer = getDeviceBufferPointer();
var ptr: c_ptr(uint(64)) = c_ptrTo(deviceBuffer);
var kernelParams = getKernelParams(ptr);
 __primitive("gpu kernel launch", funcPtr, 1, 1, 1, 1, 1, 1, 0, 0, kernelParams, 0);
output = getDataFromDevice(deviceBuffer);

writeln(output);

}

