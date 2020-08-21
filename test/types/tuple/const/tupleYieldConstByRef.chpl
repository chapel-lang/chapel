record r { var x: int = 0; }

const gTupConst = (0, new r(0));

iter yieldTupConstByConstRef() ref {
  ref result = gTupConst;
  yield result;
}

proc test1() {
  for (x, y) in yieldTupConstByConstRef() do y = new r(128);
  writeln(gTupConst);
}
test1();

proc test2() {
  for tup in yieldTupConstByConstRef() do tup[1] = new r(256);
  writeln(gTupConst);
}
test2();

