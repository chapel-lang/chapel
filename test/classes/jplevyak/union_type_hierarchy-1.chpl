union C {
  var x : integer;
}

union D {
  var x : integer;
  var y : integer;
}

function f(a : D) {
  return 1;
}

var b = D();

writeln(f(b));
