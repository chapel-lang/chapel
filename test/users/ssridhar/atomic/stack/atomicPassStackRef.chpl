proc main () {
  var z: int;
  var a: int;

  forall i in 1..1000 do
    atomic a = foo(z);

  writeln("z = ", z, " a = ", a);

  a = foo(z);

  writeln("z = ", z, " a = ", a);
}

proc foo(inout z: int): int {
  var x: int;

  atomic {
    z += 1;
    bar(x);
    x += 1;
  }
  return x;
}

proc bar(inout x: int) {
  x += 1;
}
