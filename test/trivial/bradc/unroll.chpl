
proc foo(x: int) {
  writeln("Got an int");
}

proc foo(x: real) {
  writeln("Got a real");
}

proc foo(x: string) {
  writeln("Got a string");
}

proc unrollme(x ...?numargs) {
  for param i in 0..numargs-1 {
    foo(x(i));
  }
  writeln();
}

unrollme(1,2,3,4);

unrollme(1, 2.3, "4.5.6", 78);


