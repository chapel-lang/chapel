record C {
  var x : integer;
}

record D {
  var x : integer;
  var y : integer;
}

function f(a : C) {
  return 1;
}

var b = D();

writeln(f(b));
