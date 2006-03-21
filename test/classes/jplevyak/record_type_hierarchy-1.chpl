record C {
  var x : int;
}

record D {
  var x : int;
  var y : int;
}

fun f(a : C) {
  return 1;
}

var b = C();

writeln(f(b));
