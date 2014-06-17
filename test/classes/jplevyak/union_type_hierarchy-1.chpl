union C {
  var x : int;
}

union D {
  var x : int;
  var y : int;
}

proc f(a : D) {
  return 1;
}

var b = new D();

writeln(f(b));
