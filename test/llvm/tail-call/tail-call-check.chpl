// This test verifies that tail call optimization occurs
// at all.

config const n = 10;

// CHECK: i64 @recurse
proc recurse(arg: int) : int {
  // CHECK-SAME: {
  // CHECK-NOT: @recurse
  if arg <= 0 then
    return 0;
  return 1 + recurse(arg - 1);
  // CHECK: ret i64
  // CHECK-NEXT: }
}

// CHECK: Result - 10
var x = recurse(n);
writeln("Result - ", x);
