record R {
  var x : int = 1;
  var y : int = 2;
}

var gr : R;

proc foo(in lr : R) {
  lr.x = 100;
  writeln(lr);
  writeln(gr);
}

writeln(gr);
foo(gr);
writeln(gr);
