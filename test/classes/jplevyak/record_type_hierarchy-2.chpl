record C {
  var x : int;
}

record D {
  var x : int;
  var y : int;
}

proc f(a : C) {
  return 1;
}

var b = new D();

writeln(f(b));
