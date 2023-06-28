class C { var x: int; }

proc setClt(ref lhs: borrowed C, rhs: borrowed C) lifetime lhs < rhs {
  lhs = rhs;
}
proc setClt2(ref lhs: borrowed C, rhs: borrowed C) where true lifetime lhs < rhs, lhs < rhs {
  lhs = rhs;
}
// this one never called
proc setClt2(ref lhs: borrowed C, rhs: borrowed C) where false lifetime lhs < rhs, lhs < rhs {
  lhs = rhs;
}

proc setClte(ref lhs: borrowed C, rhs: borrowed C) lifetime lhs <= rhs where true {
  lhs = rhs;
}
// this one never called
proc setClte(ref lhs: borrowed C, rhs: borrowed C) lifetime lhs <= rhs where false {
  lhs = rhs;
}

proc setCgt(ref lhs: borrowed C, rhs: borrowed C) lifetime rhs > lhs {
  lhs = rhs;
}
proc setCgte(ref lhs: borrowed C, rhs: borrowed C) lifetime rhs >= lhs {
  lhs = rhs;
}
proc setCbad(ref lhs: borrowed C, rhs: borrowed C) lifetime lhs > rhs {
  lhs = rhs;
}

proc badlt() {
  var ownB = new owned C();
  var b = ownB.borrow();
  {
    var own = new owned C();
    setClt(b, own.borrow());
  }
  writeln(b.x);
}
badlt();

proc badlt2() {
  var ownB = new owned C();
  var b = ownB.borrow();
  {
    var own = new owned C();
    setClt2(b, own.borrow());
  }
  writeln(b.x);
}
badlt2();

proc badlte() {
  var ownB = new owned C();
  var b = ownB.borrow();
  {
    var own = new owned C();
    setClte(b, own.borrow());
  }
  writeln(b.x);
}
badlte();

proc badgt() {
  var ownB = new owned C();
  var b = ownB.borrow();
  {
    var own = new owned C();
    setCgt(b, own.borrow());
  }
  writeln(b.x);
}
badgt();

proc badgte() {
  var ownB = new owned C();
  var b = ownB.borrow();
  {
    var own = new owned C();
    setCgte(b, own.borrow());
  }
  writeln(b.x);
}
badgte();

proc callsBad() {
  var ownB = new owned C();
  var b = ownB.borrow();
  {
    var own = new owned C();
    setCbad(b, own.borrow());
  }
  writeln(b.x);
}
callsBad();
