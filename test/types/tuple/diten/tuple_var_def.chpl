
proc foo(x:int, y:int, z:int) {
  return (x, y, z);
}

proc main() {

  var (x, _, z) = foo(1, 2, 3);
  //writeln(_);
  writeln(x, " ", z);
}

