use GPU, CTypes, GpuDiagnostics, Types, Random, ChplConfig;

config const verbose = false;
config param excludeForRocm = (CHPL_GPU == "amd");

// CPU versions of associated GPU functions. These aren't actually atomic but
// are used for test verification:
proc cpuAtomicAdd (ref x : ?T, val : T) : void { x += val; }
proc cpuAtomicSub (ref x : ?T, val : T) : void { x -= val; }
proc cpuAtomicExch(ref x : ?T, val : T) : void { x = val; }
proc cpuAtomicMin (ref x : ?T, val : T) : void { x = min(x, val); }
proc cpuAtomicMax (ref x : ?T, val : T) : void { x = max(x, val); }
proc cpuAtomicInc (ref x : ?T, val : T) : void { x = if x >= val then 0:T else x + 1; }
proc cpuAtomicDec (ref x : ?T, val : T) : void { x = if x == 0:T || x > val then val else x-1; }
proc cpuAtomicAnd (ref x : ?T, val : T) : void { x &= val; }
proc cpuAtomicOr  (ref x : ?T, val : T) : void { x |= val; }
proc cpuAtomicXor (ref x : ?T, val : T) : void { x ^= val; }
proc cpuAtomicCAS (ref x : ?T, cmp : T, val : T) : void { x = if x == cmp then val else x; }

proc check(orig, gpuRes, cpuRes, s) {
  stopGpuDiagnostics();
  const nLaunch = getGpuDiagnostics()[0].kernel_launch;

  if nLaunch !=1 then
    writeln(s + " didn't result in correct number of kernel launches. " +
            nLaunch:string + " launches detected.");
  else if verbose then
    writeln(s + " resulted in kernel launch");

  // To get this to work we'll need to resolve https://github.com/chapel-lang/chapel/issues/22114
  var isCorrect = true;
  for (x,y) in zip(gpuRes,cpuRes) do isCorrect &= isclose(x,y);
  var printVals = false;
  if !isCorrect {
    writeln(s + " computed wrong result. ");
    printVals = true;
  } else if verbose {
    writeln(s + " computed right result. ");
    printVals = true;
  }
  if(printVals) {
    writeln("input values: ", orig);
    writeln("gpu result: ", gpuRes);
    writeln("cpu result: ", cpuRes);
  }

  resetGpuDiagnostics();
  startGpuDiagnostics();
}

proc runTest(param op : string, type T) {
  stopGpuDiagnostics();

  var origVals : [0..2] T;
  var gpuVals  : [0..2] T;
  var cpuVals  : [0..2] T;
  var rng = createRandomStream(eltType=T, parSafe=false);
  for i in 0..2 {
    origVals[i] = rng.getNext();
    gpuVals[i] = origVals[i];
    cpuVals[i] = origVals[i];
  }

  resetGpuDiagnostics();
  startGpuDiagnostics();

  // It would be better if we could pass 'runTest' the gpu and cpu functions
  // that we want to test as first class functions instead of having this
  // repetitive switchyard. Alas, those functions are generic so (as of today)
  // can not be captured as a FCF.
  select(op) {
    when "add" {
      foreach n in 0..0 do gpuAtomicAdd(gpuVals[0],  gpuVals[1]);
      cpuAtomicAdd(cpuVals[0],  cpuVals[1]);
    }
    when "sub" {
      foreach n in 0..0 do gpuAtomicSub(gpuVals[0],  gpuVals[1]);
      cpuAtomicSub(cpuVals[0],  cpuVals[1]);
    }
    when "exch" {
      foreach n in 0..0 do gpuAtomicExch(gpuVals[0],  gpuVals[1]);
      cpuAtomicExch(cpuVals[0],  cpuVals[1]);
    }
    when "min" {
      foreach n in 0..0 do gpuAtomicMin(gpuVals[0],  gpuVals[1]);
      cpuAtomicMin(cpuVals[0],  cpuVals[1]);
    }
    when "max" {
      foreach n in 0..0 do gpuAtomicMax(gpuVals[0],  gpuVals[1]);
      cpuAtomicMax(cpuVals[0],  cpuVals[1]);
    }
    when "inc" {
      foreach n in 0..0 do gpuAtomicInc(gpuVals[0],  gpuVals[1]);
      cpuAtomicInc(cpuVals[0],  cpuVals[1]);
    }
    when "dec" {
      foreach n in 0..0 do gpuAtomicDec(gpuVals[0],  gpuVals[1]);
      cpuAtomicDec(cpuVals[0],  cpuVals[1]);
    }
    when "and" {
      foreach n in 0..0 do gpuAtomicAnd(gpuVals[0],  gpuVals[1]);
      cpuAtomicAnd(cpuVals[0],  cpuVals[1]);
    }
    when "or" {
      foreach n in 0..0 do gpuAtomicOr(gpuVals[0],  gpuVals[1]);
      cpuAtomicOr(cpuVals[0],  cpuVals[1]);
    }
    when "xor" {
      foreach n in 0..0 do gpuAtomicXor(gpuVals[0],  gpuVals[1]);
      cpuAtomicXor(cpuVals[0],  cpuVals[1]);
    }
    when "CAS" {
      foreach n in 0..0 do gpuAtomicCAS(gpuVals[0],  gpuVals[1], gpuVals[2]);
      cpuAtomicCAS(cpuVals[0],  cpuVals[1], cpuVals[2]);
    }
  }
  check(origVals, gpuVals, cpuVals, op);
}

proc main() {
  on here.gpus[0] {
    startGpuDiagnostics();

    runTest("add", c_int);  runTest("add", c_uint);  runTest("add", c_float);
    if(!excludeForRocm) {
      runTest("add", c_ulonglong);  runTest("add", c_double);
    }

    runTest("sub", c_int);  runTest("sub", c_uint);

    runTest("exch", c_int);  runTest("exch", c_uint);  runTest("exch", c_float);
    /*if !excludeForRocm then */runTest("exch", c_ulonglong);

    runTest("min", c_int);  runTest("min", c_uint);
    if !excludeForRocm then runTest("min", c_ulonglong);
    // This is not listed as supported in the ROCM (5.5) documentation:
    if(!excludeForRocm) then runTest("min", c_longlong);

    runTest("max", c_int); runTest("max", c_uint);
    if !excludeForRocm then runTest("max", c_ulonglong);
    // This is not listed as supported in the ROCM (5.5) documentation:
    if !excludeForRocm then runTest("max", c_longlong);

    runTest("inc", c_uint);

    runTest("dec", c_uint);

    runTest("and", c_int); runTest("and", c_uint);
    if !excludeForRocm then runTest("and", c_ulonglong);

    runTest("or", c_int); runTest("or", c_uint);
    if !excludeForRocm then runTest("or", c_ulonglong);

    runTest("xor", c_int); runTest("xor", c_uint);
    if !excludeForRocm then runTest("xor", c_ulonglong);

    runTest("CAS", c_int); runTest("CAS", c_uint);
    if !excludeForRocm then runTest("CAS", c_ulonglong);

    // Before adding support for this I would want better capabilities
    // to process CHPL_GPU (this is only supported when compiling for
    // CUDA with CC >= 7.0. This is also not listed as something supported
    // by the ROCM (5.5) documentation
    // if !excludeForRocm then runTest("CAS", c_ushort);
  }
}
