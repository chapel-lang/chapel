extern {
  #include <cuda.h>
  //#include <stdio.h>
  //#include <stdlib.h>
  #include <assert.h>

  #define FATBIN_FILE "tmp/chpl__gpu.fatbin"

  void checkCudaErrors(CUresult err) {
    assert(err == CUDA_SUCCESS);
  }

  double launchKernel(){
    CUdevice    device;
    CUmodule    cudaModule;
    CUcontext   context;
    CUfunction  function;
    int         devCount;

    double X;

    // CUDA initialization
    checkCudaErrors(cuInit(0));
    checkCudaErrors(cuDeviceGetCount(&devCount));
    checkCudaErrors(cuDeviceGet(&device, 0));

    char name[128];
    checkCudaErrors(cuDeviceGetName(name, 128, device));
//    printf("Using CUDA Device %s\n", name);
    //std::cout << "Using CUDA Device [0]: " << name << "\n";


    // Create driver context
    checkCudaErrors(cuCtxCreate(&context, 0, device));

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
    printf("creating module\n");
    auto err1 = cuModuleLoadData(&cudaModule, buffer);
    // checkCudaErrors(cuModuleLoad(&cudaModule, "tmp/chpl__gpu_ptx.s"));
    //auto err1 = cuModuleLoad(&cudaModule, "tmp/add_nums_kernel.ptx");
    //printf("%i\n", (int)err1);
    //checkCudaErrors(cuModuleLoad(&cudaModule, "examples/examples4/addNumsKernel.cu-cuda-nvptx64-nvidia-cuda.fatbin"));


    // Get kernel function
    printf("creating kernel\n");
    auto err = cuModuleGetFunction(&function, cudaModule, "add_nums");
    //printf("%i\n", (int)err);
    //checkCudaErrors(cuModuleGetFunction(&function, cudaModule, "add_nums"));

    CUdeviceptr devBufferX;

    printf("allocating memory\n");
    checkCudaErrors(cuMemAlloc(&devBufferX, sizeof(double)));

    //get value of X

    //printf("input num: ");
    //scanf("%lf", &X);
    //printf("number: %lf\n", X);
    X = 5.0;


    printf("cuMemcpyHtoD\n");
    checkCudaErrors(cuMemcpyHtoD(devBufferX, &X, sizeof(double)));


    unsigned blockSizeX = 1;
    unsigned blockSizeY = 1;
    unsigned blockSizeZ = 1;
    unsigned gridSizeX  = 1;
    unsigned gridSizeY  = 1;
    unsigned gridSizeZ  = 1;

    // Kernel parameters
    void *KernelParams[] = { &devBufferX };

    printf("Launch kernel\n");
    checkCudaErrors(cuLaunchKernel(function, gridSizeX, gridSizeY, gridSizeZ,
                                   blockSizeX, blockSizeY, blockSizeZ,
                                   0, NULL, KernelParams, NULL));

    // Retrieve device data
    printf("cuMemcpyDtoH\n");
    checkCudaErrors(cuMemcpyDtoH(&X, devBufferX, sizeof(double)));

    printf("output: %lf\n", X);
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
//var gpu_ptr_x = send_data_to_gpu();
//add_nums(gpu_ptr_x:c_ptr(real(64)));
//send_data_to_cpu(c_ptrTo(output), gpu_ptr_x);

writeln(output);



