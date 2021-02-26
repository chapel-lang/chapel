record r { var x: int = 0; }

const gConst = (0, new r(0));

iter yieldConstByConstRef() const ref {
  const ref result = gConst;
  yield result;
}

proc test1() {
  for (x, y) in yieldConstByConstRef() do y = new r(128);
  writeln(gConst);
}
test1();

proc test2() {
  for tup in yieldConstByConstRef() do tup[1] = new r(256);
  writeln(gConst);
}
test2();

