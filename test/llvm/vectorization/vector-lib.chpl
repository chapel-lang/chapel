
proc kernel(ref A, B, scalar) {
  // CHECK: @kernel_chpl
  foreach (a, b) in zip(A, B) {
    // CHECK: pow
    // CHECK-SAME: x double> %
    // CHECK-SAME: 2.3
    a += scalar * b**2.3;
  }
}

config const N = 1000;

proc main() {
  var A: [1..N] real;
  var B: [1..N] real;
  kernel(A, B, 2.5);
}
