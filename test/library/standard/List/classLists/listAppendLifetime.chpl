private use List;

class C { var x: int; }

proc bad1() {
  var L: list(borrowed C);

  {
    var own = new owned C();
    var bb = own.borrow();
    L.append(bb);
  }

  return;
}
bad1();

proc bad2() {
  var L: list(borrowed C);

  {
    var own = new owned C();
    L.append(own);
  }

  return;
}
bad2();

proc ok() {
  var L: list(owned C);

  {
    var own = new owned C();
    L.append(own);
  }

  return;
}
ok();

