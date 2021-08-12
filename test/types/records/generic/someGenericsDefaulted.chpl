config param errorCase = 0;

record R {
  type t = int;
  param rank;
  var x: rank*t;
}

proc foo(r: R) {
  writeln("In foo, r is: ", r);
}

var ri2: R(rank=2);
var rr2: R(real, 2);
var ri3: R(int, 3);

foo(ri2);
foo(rr2);
foo(ri3);
writeln();
