// The purpose of this test is to use the explicit "gpu threadIdx", "gpu blockIdx"
// "gpu blockDim" and "gpu gridDim" primitives.

extern {
  #include <cuda.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <stdint.h>
  #include <assert.h>

  #define VALS_PER_THREAD 13

  static void checkCudaErrors(CUresult err) {
    assert(err == CUDA_SUCCESS);
  }

  static CUdeviceptr getDeviceBufferPointer(
    int64_t gdimX, int64_t gdimY, int64_t gdimZ,
    int64_t bdimX, int64_t bdimY, int64_t bdimZ)
  {
    int64_t N = gdimX * gdimY * gdimZ * bdimX * bdimY * bdimZ * VALS_PER_THREAD;

    double X[N];
    CUdeviceptr devBufferX;

    checkCudaErrors(cuMemAlloc(&devBufferX, sizeof(double)*N));

    for(int i = 0; i < N; i++) {
      X[i] = 999;
    }
    
    checkCudaErrors(cuMemcpyHtoD(devBufferX, &X, sizeof(double)*N));

    return devBufferX;
  }

  static void **getKernelParams(CUdeviceptr *devBufferX){
    static void* kernelParams[1];
    kernelParams[0] = devBufferX;
    return kernelParams;
  }

  static void getAndPrintDataFromDevice(CUdeviceptr devBufferX, 
    int64_t gdimX, int64_t gdimY, int64_t gdimZ,
    int64_t bdimX, int64_t bdimY, int64_t bdimZ)
  {
    int64_t N = gdimX * gdimY * gdimZ * bdimX * bdimY * bdimZ * VALS_PER_THREAD;

    double X[N];
    cuMemcpyDtoH(&X, devBufferX, sizeof(double)*N);

    printf("     |--- thread idx --|--- block idx ---|--- block size --|--- grid size --|\n");
    printf("  idx tid_x tid_y tid_z bid_x bid_y bid_z bdm_x bdm_y bdm_z gdm_x gdm_y gdm_z\n");

    for(int i = 0; i < N/VALS_PER_THREAD; i++) {
      for(int j = 0; j < VALS_PER_THREAD; j++) {
        printf("%5.0f ", X[i*VALS_PER_THREAD + j]);
      }
      printf("\n");
    }
  }
}

pragma "codegen for GPU"
pragma "always resolve function"
export proc add_nums(dst_ptr: c_ptr(real(64))){
  var tid_x = __primitive("gpu threadIdx x");
  var tid_y = __primitive("gpu threadIdx y");
  var tid_z = __primitive("gpu threadIdx z");
  var bid_x = __primitive("gpu blockIdx x");
  var bid_y = __primitive("gpu blockIdx y");
  var bid_z = __primitive("gpu blockIdx z");
  var blockDim_x = __primitive("gpu blockDim x");
  var blockDim_y = __primitive("gpu blockDim y");
  var blockDim_z = __primitive("gpu blockDim z");
  var gridDim_x  = __primitive("gpu gridDim x");
  var gridDim_y  = __primitive("gpu gridDim y");
  var gridDim_z  = __primitive("gpu gridDim z");

  const VALS_TO_REPORT = 13;

  var idx = VALS_TO_REPORT *
    ((tid_z + blockDim_z * bid_z)  * (gridDim_x * blockDim_x * gridDim_y * blockDim_y) +
     (tid_y + blockDim_y * bid_y)  * (gridDim_x * blockDim_x) +
     (tid_x + blockDim_x * bid_x));

  dst_ptr[idx]    = idx;
  dst_ptr[idx+1]  = tid_x;
  dst_ptr[idx+2]  = tid_y;
  dst_ptr[idx+3]  = tid_z;
  dst_ptr[idx+4]  = bid_x;
  dst_ptr[idx+5]  = bid_y;
  dst_ptr[idx+6]  = bid_z;
  dst_ptr[idx+7]  = blockDim_x;
  dst_ptr[idx+8]  = blockDim_y;
  dst_ptr[idx+9]  = blockDim_z;
  dst_ptr[idx+10] = gridDim_x;
  dst_ptr[idx+11] = gridDim_y;
  dst_ptr[idx+12] = gridDim_z;
}

// gdim = grid dimension; bdim = block dimension
proc runExample(gdimX, gdimY, gdimZ, bdimX, bdimY, bdimZ) {
  writeln(" Grid size: ", gdimX, " x ", gdimY, " x ", gdimZ);
  writeln("Block size: ", bdimX, " x ", bdimY, " x ", bdimZ);

  var deviceBuffer = getDeviceBufferPointer(gdimX, gdimY, gdimZ, bdimX, bdimY, bdimZ);
  __primitive("gpu kernel launch", c"add_nums",
              gdimX, gdimY, gdimZ, bdimX, bdimY, bdimZ,
              deviceBuffer);
  getAndPrintDataFromDevice(deviceBuffer, gdimX, gdimY, gdimZ, bdimX, bdimY, bdimZ);

  writeln();
}

proc main() {
  on here.getChild(1) {
    var dummy = [1,2,3]; // to ensure that the CUDA context is attached to the
                         // thread

    runExample(1,1,1, 1,1,1);

    runExample(1,1,1, 2,2,2);
    runExample(1,1,1, 2,3,4);

    runExample(2,2,2, 1,1,1);
    runExample(2,3,4, 1,1,1);

    runExample(2,2,2, 2,2,2);
    runExample(2,3,4, 2,3,4);
  }
}

