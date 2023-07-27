module GpuTestCommon {
  use GpuDiagnostics;

  config const printGpuDiags = false;

  proc assertGpuDiags(umLaunch, aodLaunch, host_to_device=-1, device_to_host=-1, device_to_device=-1) {
    use ChplConfig;

    if printGpuDiags {
      writeln(getGpuDiagnostics());
    }

    param isUm = CHPL_GPU_MEM_STRATEGY == "unified_memory";

    const expected = if isUm then umLaunch else aodLaunch;

    const diags = getGpuDiagnostics()[0];

    const actual = diags.kernel_launch;
    assert(actual == expected,
           "observed ", actual, " launches instead of ", expected);

    if !isUm {
      assert(h2dComm<0 || diags.host_to_device == h2dComm,
             "observed ", diags.host_to_device,
             " host-to-device comm instead of ", h2dComm);

      assert(d2hComm<0 || diags.device_to_host == d2hComm,
             "observed ", diags.device_to_host,
             " device-to-host comm instead of ", d2hComm);

      assert(d2dComm<0 || diags.device_to_device == d2dComm,
             "observed ", diags.device_to_device,
             " device-to-device comm instead of ", d2dComm);
    }

  }
}
