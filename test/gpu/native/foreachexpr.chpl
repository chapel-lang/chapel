use GpuDiagnostics;

proc double(x) do return x * 2;

on here.gpus[0] {
  startGpuDiagnostics();
  var A = for i in 1..10 do i * 2;
  stopGpuDiagnostics();
  assertGpuDiags(kernel_launch = 0);

  startGpuDiagnostics();
  var B = foreach i in 1..10 do i * 2;
  stopGpuDiagnostics();
  assertGpuDiags(kernel_launch = 1);

  startGpuDiagnostics();
  var C = forall i in 1..10 do i * 2;
  stopGpuDiagnostics();
  assertGpuDiags(kernel_launch = 2);

  startGpuDiagnostics();
  var D = double(for i in 1..10 do i * 2);
  stopGpuDiagnostics();
  assertGpuDiags(kernel_launch = 2);

  startGpuDiagnostics();
  var E = double(foreach i in 1..10 do i * 2);
  stopGpuDiagnostics();
  assertGpuDiags(kernel_launch = 3);

  startGpuDiagnostics();
  var F = double(forall i in 1..10 do i * 2);
  stopGpuDiagnostics();
  assertGpuDiags(kernel_launch = 4);
}
