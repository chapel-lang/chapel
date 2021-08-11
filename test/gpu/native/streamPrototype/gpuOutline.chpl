// this module name is hard-coded in the compiler
module GPUOutlineTest {
  extern {
    #include <cuda.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <assert.h>

    static void checkCudaErrors(CUresult err) {
      if (err != CUDA_SUCCESS) {
        fprintf(stderr, "CUDA Error code: %d\n", err);
        assert(false);
      }
    }

    static CUdeviceptr getDeviceBufferPointer(double X){
      CUdeviceptr devBufferX;

      /*cuMemAlloc(&devBufferX, sizeof(double));*/
      checkCudaErrors(cuMemAlloc(&devBufferX, sizeof(double)));
      
      checkCudaErrors(cuMemcpyHtoD(devBufferX, &X, sizeof(double)));

      return devBufferX;
    }

    static double getDataFromDevice(CUdeviceptr devBufferX){
      double X;
      checkCudaErrors(cuMemcpyDtoH(&X, devBufferX, sizeof(double)));
      return X;
    }
  }

  record GPUAbleValue {
    var value;
    forwarding var data: _ddata(value.type);

    proc init(value) {
      this.value = value;

      var devPtr = getDeviceBufferPointer(value:real);
      this.data = __primitive("cast", _ddata(value.type), devPtr);
    }

    proc getValue() {
      var dataOnHost = getDataFromDevice(__primitive("cast", CUdeviceptr, data));
      return dataOnHost;
    }
  }

  proc testMain() {
    chpl_gpu_init();
    var a = new GPUAbleValue(value=5.0);
    var b = new GPUAbleValue(value=10.0);

    var initialRHS = b.getValue();
    var initialLHS = a.getValue();

    forall i in 0..0 {
      a[i] = b[i];
    }

    var finalLHS = a.getValue();

    writeln("initialRHS = ", initialRHS);
    writeln("initialLHS = ", initialLHS);
    writeln("finalLHS   = ", finalLHS);
  }

  testMain();
}
