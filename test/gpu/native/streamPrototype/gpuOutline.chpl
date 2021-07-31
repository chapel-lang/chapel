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

    static void **getKernelParams(CUdeviceptr *devBufferX){
      static void* kernelParams[1];
      kernelParams[0] = devBufferX;
      return kernelParams;
    }


    static double getDataFromDevice(CUdeviceptr devBufferX){
      double X;
      checkCudaErrors(cuMemcpyDtoH(&X, devBufferX, sizeof(double)));
      return X;
    }

  }

  config param allocOnGpu = true;

  record GPUAbleArray {
    type eltType;
    var size = 0: int;
    forwarding var data: _ddata(eltType);

    proc postinit() {
      var dummy = false;
      if allocOnGpu {
        var devPtr = getDeviceBufferPointer(size:real);
        data = __primitive("cast", c_ptr(eltType), devPtr):data.type;
      }
      else
        data = _ddata_allocate_noinit(eltType, size, dummy);
    }

    proc getHostArray() {
      if allocOnGpu {
        var dataOnHost = getDataFromDevice(__primitive("cast", CUdeviceptr, data));
        return dataOnHost;
      }
      else {
        compilerError("This doesn't make much sense");
      }
    }
  }

  proc testMain() {
    chpl_gpu_init();

    var a = new GPUAbleArray(real, 5);
    var b = new GPUAbleArray(real, 10);

    /*if !allocOnGpu {*/
      /*for i in 0..9 do a[i] = 1;*/
      /*for i in 0..9 do b[i] = 2;*/
    /*}*/

    var rhs = b.getHostArray();
    var lhs = a.getHostArray();

    forall i in 0..0 {
      a[i] = b[i];
    }

    var result = a.getHostArray();

    writeln(rhs, " ", lhs, " ", result);

    writeln("Hello world");
  }

  testMain();
  /*writeln();*/
}
