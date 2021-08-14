extern {
  #include <cuda.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <assert.h>

  static void checkCudaErrors(CUresult err) {
    assert(err == CUDA_SUCCESS);
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
chpl_gpu_init();

var output: real(64);
var deviceBuffer = getDeviceBufferPointer();

// arguments are: function name, grid size, block size, number of arguments
__primitive("gpu kernel launch flat", c"add_nums", 1, 1, deviceBuffer);
output = getDataFromDevice(deviceBuffer);

writeln(output);
}

