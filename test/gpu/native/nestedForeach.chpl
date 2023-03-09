use GpuDiagnostics;
use GPU;

config const low = 0, high = -1;

startGpuDiagnostics();
on here.gpus[0] {
  var A: [1..10, 1..10] int;
  foreach a in A {
    assertOnGpu();
    a += 1;
  }
  writeln(A);
  var B: [1..10, 1..10] real;
  foreach (i,j) in {1..10, 1..10} {
    assertOnGpu();
    B(i,j) = i + j;
  }
  writeln(B);
  // foreach (i,j) in {1..-1, 1..-1} {
  //   assertOnGpu();
  // }
  // foreach (i,j) in {low..high, low..high} {
  //   assertOnGpu();
  // }
}
stopGpuDiagnostics();
writeln(getGpuDiagnostics());
