record bar {
  type t;
  var x : t;
}

var b : bar(int);

writeln(b);

proc foo(b : bar(int)) {
  writeln(b);
}

foo(b);
