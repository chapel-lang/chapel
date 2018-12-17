class C { var x: int; }

proc setClt(ref lhs: C, rhs: C) lifetime lhs < rhs {
  lhs = rhs;
}
proc setClt2(ref lhs: C, rhs: C) where true lifetime lhs < rhs, lhs < rhs {
  lhs = rhs;
}
// this one never called
proc setClt2(ref lhs: C, rhs: C) where false lifetime lhs < rhs, lhs < rhs {
  lhs = rhs;
}

proc setClte(ref lhs: C, rhs: C) lifetime lhs <= rhs where true {
  lhs = rhs;
}
// this one never called
proc setClte(ref lhs: C, rhs: C) lifetime lhs <= rhs where false {
  lhs = rhs;
}

proc setCgt(ref lhs: C, rhs: C) lifetime rhs > lhs {
  lhs = rhs;
}
proc setCgte(ref lhs: C, rhs: C) lifetime rhs >= lhs {
  lhs = rhs;
}
proc setCbad(ref lhs: C, rhs: C) lifetime lhs > rhs {
  lhs = rhs;
}

proc badlt() {
  var b: borrowed C;
  {
    var own = new owned C();
    setClt(b, own.borrow());
  }
  writeln(b.x);
}
badlt();

proc badlt2() {
  var b: borrowed C;
  {
    var own = new owned C();
    setClt2(b, own.borrow());
  }
  writeln(b.x);
}
badlt2();

proc badlte() {
  var b: borrowed C;
  {
    var own = new owned C();
    setClte(b, own.borrow());
  }
  writeln(b.x);
}
badlte();

proc badgt() {
  var b: borrowed C;
  {
    var own = new owned C();
    setCgt(b, own.borrow());
  }
  writeln(b.x);
}
badgt();

proc badgte() {
  var b: borrowed C;
  {
    var own = new owned C();
    setCgte(b, own.borrow());
  }
  writeln(b.x);
}
badgte();

proc callsBad() {
  var b: borrowed C;
  {
    var own = new owned C();
    setCbad(b, own.borrow());
  }
  writeln(b.x);
}
callsBad();


