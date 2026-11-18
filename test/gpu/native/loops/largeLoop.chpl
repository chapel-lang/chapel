use GPU;
use GpuDiagnostics;

config const n = max(int(32))+1;
config const printArr = false;
config const printChecksum = false;

var AHost: [0..<n] uint(8);

on here.gpus[0] {
  var A: [0..<n] uint(8);
  @assertOnGpu
  foreach i in 0..<n {
    A[i] = 1;
  }
  AHost = A;
}

// reduction doesn't work on host either?
var sum = 0;
for a in AHost do sum += a;

assert(sum == n);

if printChecksum {
  writeln(n);
  writeln(sum);
}

if printArr then writeln(AHost);
