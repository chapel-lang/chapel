use CTypes;
use GpuDiagnostics;

extern {
  #include <cuda_runtime.h>

  static __device__ __host__ void printIt(const char *msg, int idx) {
    printf("%s: %i\n", msg, idx);
  }
}

pragma "codegen for GPU"
pragma "always resolve function"
extern proc printIt(msg : c_ptrConst(c_char), idx : c_int) : void;

proc doit() {
  var x = 100;
  writeln("CPU  BEFORE ALL: ", x);
  foreach i in 0..3 with (in x) {
    printIt("CPU  BEFORE":c_ptrConst(c_char), x : c_int);
    x = i;
    printIt("CPU  AFTER":c_ptrConst(c_char), x : c_int);
  }
  writeln("CPU  AFTER ALL: ", x);

  writeln();

  on here.gpus[0] {
    var x = 100;
    writeln("GPU  BEFORE ALL: ", x);
    @assertOnGpu
    foreach i in 0..3 with (in x) {
      printIt("GPU  BEFORE":c_ptrConst(c_char), x : c_int);
      x = i;
      printIt("GPU  AFTER":c_ptrConst(c_char), x : c_int);
    }
    writeln("GPU  AFTER ALL: ", x);
  }
}

doit();
