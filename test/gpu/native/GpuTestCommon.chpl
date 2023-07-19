module GpuTestCommon {
  use GpuDiagnostics;

  config const printGpuDiags = false;

  proc verifyDiags(um, aod, h2d=-1, d2h=-1, d2d=-1) {
    use ChplConfig;

    if printGpuDiags {
      writeln(getGpuDiagnostics());
    }

    param isUm = CHPL_GPU_MEM_STRATEGY == "unified_memory";

    const expected = if isUm then um else aod;

    const diags = getGpuDiagnostics()[0];

    const actual = diags.kernel_launch;
    assert(actual == expected,
           "observed ", actual, " launches instead of ", expected);

    if !isUm {
      assert(h2d<0 || diags.host_to_device == h2d,
             "observed ", diags.host_to_device,
             " host-to-device comm instead of ", h2d);

      assert(d2h<0 || diags.device_to_host == d2h,
             "observed ", diags.device_to_host,
             " device-to-host comm instead of ", d2h);

      assert(d2d<0 || diags.device_to_device == d2d,
             "observed ", diags.device_to_device,
             " device-to-device comm instead of ", d2d);
    }

  }
}
