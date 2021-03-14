record r { var x: int = 0; }

var gVar = (0, new r(0));

iter yieldVarByConstRef() const ref {
  const ref result = gVar;
  yield result;
}

proc test1() {
  for (x, y) in yieldVarByConstRef() do y = new r(128);
  writeln(gVar);
}
test1();

proc test2() {
  for tup in yieldVarByConstRef() do tup[1] = new r(256);
  writeln(gVar);
}
test2();

