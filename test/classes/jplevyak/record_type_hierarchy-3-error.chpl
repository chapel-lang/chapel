record C {
  var x : integer;
}

record D {
  var x : integer;
  var y : integer;
}

function f(a : D) {
  return 1;
}

var b = C();

writeln(f(b));
