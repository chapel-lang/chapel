proc f() ref {
  var A:[1..4] int;
  return A[2..3];
}

writeln(f());


