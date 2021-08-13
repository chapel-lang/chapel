extern {
  #include <cuda.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <assert.h>

  static void checkCudaErrors(CUresult err, int num) {
    if (err != CUDA_SUCCESS) {
      fprintf(stderr, "CUDA Error code: %d at %d\n", err, num);
      assert(false);
    }
  }

  static CUdeviceptr getDeviceBufferPointer(double* X, int64_t size) {
    CUdeviceptr devBufferX;

    checkCudaErrors(cuMemAlloc(&devBufferX, sizeof(double) * size), 1);
    checkCudaErrors(cuMemcpyHtoD(devBufferX, X, sizeof(double) * size),2);

    return devBufferX;
  }

  static double* getDataFromDevice(CUdeviceptr devBufferX, int64_t size){
    double *X = (double*)malloc(sizeof(double) * size);
    checkCudaErrors(cuMemcpyDtoH(X, devBufferX, sizeof(double) * size), 3);
    return X;
  }
}

record GPUAbleValue {
  var value;
  var size : int;
  forwarding var data: _ddata(real);

  proc init(value) {
    this.value = value;
    this.size = value.size;

    var devPtr = getDeviceBufferPointer(c_ptrTo(value[0]), value.size);
    this.data = __primitive("cast", _ddata(real), devPtr);
  }

  proc getValue() {
    var dataOnHost = getDataFromDevice(__primitive("cast", CUdeviceptr, data), size);
    return dataOnHost;
  }
}

/ ----------------------------------------------------------------------------

// this module name is hard-coded in the compiler
module GPUOutlineTest {
  use CPtr;

  proc CArrayToStr(arr, size) {
    var str = "";
    var first = true;
    for i in 0..<size {
      if !first then
        str += ", ";
      else
        first = false;
      str += __primitive("cast", c_ptr(real), arr)[i] : string;
    }
    return str;
  }
  proc testMain(a,b) {
    var initialRHS = b.getValue();
    var initialLHS = a.getValue();

    forall i in 0..4 {
      a[i] = b[i];
    }

    var finalLHS = a.getValue();

    writeln("initialRHS = ", CArrayToStr(initialRHS, a.size));
    writeln("initialLHS = ", CArrayToStr(initialLHS, a.size));
    writeln("finalLHS   = ", CArrayToStr(finalLHS, a.size));
  }
}

use GPUOutlineTest, CPtr;

chpl_gpu_init();
var a = new GPUAbleValue(value=[0.0, 0.0, 0.0, 0.0]);
var b = new GPUAbleValue(value=[111.1, 222.2, 333.3, 444.4]);
testMain(a,b);
