// Verify 'if chpl_cpuVsGpuToken' is param-folded for non-GPU computations.

proc test() {
  if chpl_cpuVsGpuToken {
    writeln("ok");
  } else {
    compilerError("this line should be compiled only for loc.model=gpu");
  }
  return numLocales + 1;
}

compilerError(test().type:string);
