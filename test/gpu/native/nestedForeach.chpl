use GpuDiagnostics;
use GPU;


config const low = 0, high = -1;

startGpuDiagnostics();
on here.gpus[0] {
  var A: [1..10, 1..10] int;
  @assertOnGpu
  foreach a in A {
    a += 1;
  }
  writeln(A);
  var B: [1..10, 1..10] real;
  @assertOnGpu
  foreach (i,j) in {1..10, 1..10} {
    B(i,j) = i + j;
  }
  writeln(B);
  @assertOnGpu
  foreach (i,j) in {1..-1, 1..-1} {}
  @assertOnGpu
  foreach (i,j) in {low..high, low..high} {}
}
stopGpuDiagnostics();
assertGpuDiags(kernel_launch_um=2, kernel_launch_aod=4);
