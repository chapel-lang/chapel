// This test checks the basic functionality of
// 'chpl_cpuVsGpuToken' and pragma "not called from gpu"
// which are not user-facing.

// The .good files also lock the current error behavior when i==err1 or err2.
// We intend to improve it going forward so .good will need to change.

config const err1=0, err2=0, err3=0, err4=0;
config const n = 5;
var space = {1..n};

on here.gpus[0] {
  writeln("{ start 'on'");

  @assertOnGpu
  forall i in space {
    chpl_gpu_printf0("'on' on gpu - 1\n");
    if i == err1 then notRunOnGPUs(); // error if invoked
  }
  notRunOnGPUs();

  @assertOnGpu
  forall i in space {
    chpl_gpu_printf0("'on' on gpu - 2\n");
    if i == err2 then chpl_gpu_printf1("gpu: cpuInteger=%d\n", cpuInteger());
  }
  writeln("cpuInteger = ", cpuInteger());

  writeln("} finish 'on'");
}

on here.gpus[0] {
  either();
}

either();

proc either() {
  writeln("{ start either()");

  forall i in space {
    if chpl_cpuVsGpuToken {
      notRunOnGPUs(); // on cpu - should be OK
    } else {
      chpl_gpu_printf0("either() on gpu - 1\n");
      if i == err3 then notRunOnGPUs(); // error if invoked
    }
  }

  forall i in space {
    if chpl_cpuVsGpuToken {
      printf("cpu: cpuInteger=%d\n", cpuInteger()); // on cpu - should be OK
    } else {
      chpl_gpu_printf0("either() on gpu - 2\n");
      if i == err4 then chpl_gpu_printf1("gpu: cpuInteger=%d\n", cpuInteger());
    }
  }

  writeln("} finish either()");
}

pragma "not called from gpu"
proc notRunOnGPUs() {
  // note that printf would not compile during the GPU phase of codegen
  printf("notRunOnGPUs()\n");
}

pragma "not called from gpu"
proc cpuInteger() {
  return 12345;
}

extern proc printf(args...);

pragma "codegen for CPU and GPU"
extern proc chpl_gpu_printf0(fmt) : void;

pragma "codegen for CPU and GPU"
extern proc chpl_gpu_printf1(fmt, x1) : void;
