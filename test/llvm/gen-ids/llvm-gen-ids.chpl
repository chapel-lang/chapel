// This test just verifies that chpl.ast.id metadata
// is emitted (at all) on functions and instructions when
// --gen-ids is specified.

config const n = 1;
config const fac = 4;

// CHECK: i64 @llvm_HYPHEN_gen_HYPHEN_ids_factorial
// CHECK-SAME: !chpl.ast.id
proc factorial(arg: int): int {
  var ret = arg;
  if arg >= 2 {
    ret *= factorial(arg-1);
  }
  return ret;
}

proc call_factorial()  {
  // CHECK: i64 @llvm_HYPHEN_gen_HYPHEN_ids_call_factorial
  // CHECK: @llvm_HYPHEN_gen_HYPHEN_ids_factorial
  // CHECK-SAME: !chpl.ast.id
  var x = factorial(fac);
  return x;
}

var x = call_factorial();
// CHECK: Result=24
writeln("Result=", x);
