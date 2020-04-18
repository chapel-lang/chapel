import List.list;

class C { var x: int = 0; }

var lstGlobal: list(borrowed C);

proc passRefListAndAppend(ref lst) {
  var own = new owned C(64);
  var x = own.borrow();
  var y = new borrowed C(128);

  // This should be a lifetime error.
  lst.append(x);
  lst.append(y);

  return;
}

proc test1() {
  passRefListAndAppend(lstGlobal);
  writeln(lstGlobal);

  return;
}
test1();

proc test2() {
  var lst: list(borrowed C);

  passRefListAndAppend(lst);
  writeln(lst);

  return;
}
test2();

