// This test verifies that a default argument that is promoted
// executes once per iteration.
//
// This was discussed / decided upon in issue #7889.

proc makeit() {
  writeln("IN makeit");
  return 1;
}

proc foo(x:int, y=makeit()) {
  return x+y;
}

var A = [1,2,3,4,5,6,7,8,9];
writeln(A);
var B = foo(A);
writeln(B);
