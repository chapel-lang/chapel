record R {
  var x : integer = 1;
  var y : integer = 2;
}

var gr : R;

function foo(lr : R) {
  lr.x = 100;
  writeln(lr);
  writeln(gr);
}

writeln(gr);
foo(gr);
writeln(gr);
