use GpuDiagnostics;

record innerArray {
  type t;
  var D = {0..-1};
  forwarding var A: [D] t;

  proc ref resize(size: int) {
    this.D = {0..#size};
  }
}

on here.gpus[0] {
  startGpuDiagnostics();

  var A : innerArray(uint(32));
  A.resize(1);

  stopGpuDiagnostics();
  assertGpuDiags(kernel_launch_um=0, kernel_launch_aod=1);
}
