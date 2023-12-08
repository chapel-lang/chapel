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


iter myIter() {
  var __CFL_IDX__ : int;
  while __primitive("C for loop",
          __primitive( "=", __CFL_IDX__, 0),
          __primitive("<=", __CFL_IDX__, 0),
          __primitive("+=", __CFL_IDX__, 1))
  {
    yield __CFL_IDX__;
  }

  while __primitive("C for loop",
          __primitive( "=", __CFL_IDX__, 1),
          __primitive("<=", __CFL_IDX__, 1),
          __primitive("+=", __CFL_IDX__, 1))
  {
    yield __CFL_IDX__;
  }
}

// --------------

proc doit() {
  writeln("ON CPU ---");
  var __VAR__ = 100;
  writeln("BEFORE ALL: ", __VAR__);
  foreach __IDX__ in myIter() with (in __VAR__) {
    printIt("BEFORE":c_ptrConst(c_char), __VAR__ : c_int);
    __VAR__ = __IDX__;
    printIt("AFTER":c_ptrConst(c_char), __VAR__ : c_int);
  }
  writeln("AFTER ALL: ", __VAR__);

  writeln();

  on here.gpus[0] {
    writeln("ON GPU ---");
    var __VAR__ = 100;
    writeln("BEFORE ALL: ", __VAR__);
    foreach __IDX__ in myIter() with (in __VAR__) {
      printIt("BEFORE":c_ptrConst(c_char), __VAR__ : c_int);
      __VAR__ = __IDX__;
      printIt("AFTER":c_ptrConst(c_char), __VAR__ : c_int);
    }
    writeln("AFTER ALL: ", __VAR__);
  }
}

doit();
