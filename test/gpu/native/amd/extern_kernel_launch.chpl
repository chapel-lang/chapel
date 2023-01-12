extern {
  #include <hip/hip_runtime.h>
  #include <hip/hip_runtime_api.h>
  #include <assert.h>

  extern char* chpl_gpuBinary;

  #pragma GCC diagnostic ignored "-Wdeprecated-declarations"

  static double launchKernel(){
    hipDevice_t    device;
    hipModule_t    module;
    hipFunction_t  function;
    int            devCount;
    hipCtx_t       context = NULL;

    double X;

    hipCtxGetCurrent(&context);

    if (context == NULL) {
      hipInit(0);
      hipGetDeviceCount(&devCount);
      hipDeviceGet(&device, 0);

      char name[128];
      hipDeviceGetName(name, 128, device);
      hipCtxCreate(&context, 0, device);
    }

    /*
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
    */


    hipModuleLoadData(&module, chpl_gpuBinary);
    hipModuleGetFunction(&function, module, "add_nums");

    hipDeviceptr_t devBufferX;
    hipMalloc(&devBufferX, sizeof(double));

    srand(time(NULL));
    X = rand() % 100;

    hipMemcpyHtoD(devBufferX, &X, sizeof(double));

    unsigned blockSizeX = 1;
    unsigned blockSizeY = 1;
    unsigned blockSizeZ = 1;
    unsigned gridSizeX  = 1;
    unsigned gridSizeY  = 1;
    unsigned gridSizeZ  = 1;

    void *KernelParams[] = { &devBufferX };

    hipModuleLaunchKernel(function, gridSizeX, gridSizeY, gridSizeZ,
                                   blockSizeX, blockSizeY, blockSizeZ,
                                   0, NULL, KernelParams, NULL);
    hipMemcpyDtoH(&X, devBufferX, sizeof(double));

    return X;

  }

}

pragma "codegen for GPU"
pragma "always resolve function"
export proc add_nums(dst_ptr: c_ptr(real(64))){
  //var a = a*a+10;
  dst_ptr[0] = dst_ptr[0] * dst_ptr[0] + 10;
}

var output: real(64);
output = launchKernel();
writeln(output);
