use GpuDiagnostics;
use GPU;


config const low = 0, high = -1;

startGpuDiagnostics();
on here.gpus[0] {
  var A: [1..10, 1..10] int;
  @assertOnGpu
  forall a in A {
    a += 1;
  }
  writeln(A);
  var B: [1..10, 1..10] real;
  @assertOnGpu
  forall (i,j) in {1..10, 1..10} with (ref B) {
    B(i,j) = i + j;
  }
  writeln(B);
  @assertOnGpu
  forall (i,j) in {1..-1, 1..-1} {}
  @assertOnGpu
  forall (i,j) in {low..high, low..high} {}
}
stopGpuDiagnostics();
assertGpuDiags(kernel_launch_um=2, kernel_launch_aod=4);
