
proc ifMask(ref Res, A, B) {
  // CHECK-LABEL: @ifMask_chpl
  // CHECK-SAME: %Res{{[0-9a-zA-Z_.]*}},
  // CHECK-SAME: %A{{[0-9a-zA-Z_.]*}},
  // CHECK-SAME: %B{{[0-9a-zA-Z_.]*}})

  foreach i in A.domain {
    // CHECK: tail call <{{[0-9]+}} x double> @llvm.masked.load
    Res[i] = if A[i] >= 0:A.eltType then B[i] else 0:A.eltType;
  }
}

config const N = 10;
proc main() {
  var A: [1..N] real = 1..N;
  var B: [1..N] real = 1..N;
  var Res: [1..N] real;

  ifMask(Res, A, B);
  writeln("ifMask: ", Res);
}
