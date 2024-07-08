// This was a bug reported in the following issue:
//
//   https://github.com/chapel-lang/chapel/issues/23168
//
// In this issue, an FCF type returning a tuple of more than 2 elements
// resulted in an internal compiler error. This is a regression test for that issue.

proc bar(x: int, y: int, z: int): 3*int { return (x, y, z); }
type Tbar = proc(x: int, y: int, z: int): 3*int; // used to break at compile-time

var barFcf: Tbar = bar;
writeln(barFcf(1,2,3));
