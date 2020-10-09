record r { var x: int = 0; }

proc modifyNestedRefTupleElem(ref tup) {
  tup[0] = 1;
  tup[1][0] = 2;
  tup[1][1] = 3;
}

proc test1() {
  var tup = (0, (0, 0));
  ref x = tup;
  writeln(tup);
  modifyNestedRefTupleElem(x);
  writeln(tup);
}
test1();

