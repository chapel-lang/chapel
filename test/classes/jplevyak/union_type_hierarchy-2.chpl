union C {
  var x : int;
}

union D {
  var x : int;
  var y : int;
}

function f(a : D) {
  return 1;
}

var b = C();

writeln(f(b));
