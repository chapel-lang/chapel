record r { var x: int = 0; }

const gElem = new r(0);

// TODO: What does it mean to return a tuple by `ref`? What is even going
// on here? Time to investigate...
iter yieldConstElemByRef() ref {
  yield (0, gElem);
}

// TODO: Still prints 0 instead of 256, why?
proc test1() {
  for (x, y) in yieldConstElemByRef() do y = new r(256);
  writeln(gElem);
}
test1();

proc test2() {
  for tup in yieldConstElemByRef() do tup[1] = new r(512);
  writeln(gElem);
}
test2();

