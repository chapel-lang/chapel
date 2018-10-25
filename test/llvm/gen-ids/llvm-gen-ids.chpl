// This test just verifies that chpl.ast.id metadata
// is emitted (at all) on functions and instructions when
// --gen-ids is specified.

config const n = 1;

// CHECK: i64 @factorial
// CHECK-SAME: !chpl.ast.id
proc factorial(arg: int): int {
  var ret = arg;
  if arg >= 2 {
    // CHECK: @factorial
    // CHECK-SAME: !chpl.ast.id
    ret *= factorial(arg-1);
  }
  return ret;
}

var x = factorial(4);
// CHECK: Result=24
writeln("Result=", x);
