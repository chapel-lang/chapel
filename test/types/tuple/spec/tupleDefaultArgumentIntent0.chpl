record R { var x: int = 0; }

var i: int;
var r: R;

test((i, r)); // The int i is copied here, but r is not.

proc test(tup) {
  tup[1] = 1;
  tup[2].x = 3;
}

writeln(i); // Outputs 0.
writeln(r); // Outputs (x = 3).
