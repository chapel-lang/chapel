module GpuTestCommon {
  use GpuDiagnostics;

  config const printGpuDiags = false;

  proc verifyLaunches(um, aod) {
    use ChplConfig;

    if printGpuDiags {
      writeln(getGpuDiagnostics());
    }

    const expected = if CHPL_GPU_MEM_STRATEGY == "unified_memory"
                        then um else aod;
    const actual = getGpuDiagnostics()[0].kernel_launch;
    assert(actual == expected,
           "observed ", actual, " launches instead of ", expected);

  }
}
