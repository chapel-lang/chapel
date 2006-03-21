record C {
  var x : int;
}

record D {
  var x : int;
  var y : int;
}

fun f(a : D) {
  return 1;
}

var b = C();

writeln(f(b));
