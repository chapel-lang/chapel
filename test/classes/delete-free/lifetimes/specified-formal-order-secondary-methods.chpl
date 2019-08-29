class C { var x: int; }

proc ref C.setClt(rhs: borrowed C) lifetime this < rhs {
  this = rhs;
}
proc ref C.setClt2(rhs: borrowed C) where true lifetime this < rhs, this < rhs {
  this = rhs;
}
// this one never called
proc ref C.setClt2(rhs: borrowed C) where false lifetime this < rhs, this < rhs {
  this = rhs;
}

proc ref C.setClte(rhs: borrowed C) lifetime this <= rhs where true {
  this = rhs;
}
// this one never called
proc ref C.setClte(rhs: borrowed C) lifetime this <= rhs where false {
  this = rhs;
}

proc ref C.setCgt(rhs: borrowed C) lifetime rhs > this {
  this = rhs;
}
proc ref C.setCgte(rhs: borrowed C) lifetime rhs >= this {
  this = rhs;
}
proc ref C.setCbad(rhs: borrowed C) lifetime this > rhs {
  this = rhs;
}

proc badlt() {
  var b = new borrowed C();
  {
    var own = new owned C();
    b.setClt(own.borrow());
  }
  writeln(b.x);
}
badlt();

proc badlt2() {
  var b = new borrowed C();
  {
    var own = new owned C();
    b.setClt2(own.borrow());
  }
  writeln(b.x);
}
badlt2();

proc badlte() {
  var b = new borrowed C();
  {
    var own = new owned C();
    b.setClte(own.borrow());
  }
  writeln(b.x);
}
badlte();

proc badgt() {
  var b = new borrowed C();
  {
    var own = new owned C();
    b.setCgt(own.borrow());
  }
  writeln(b.x);
}
badgt();

proc badgte() {
  var b = new borrowed C();
  {
    var own = new owned C();
    b.setCgte(own.borrow());
  }
  writeln(b.x);
}
badgte();

proc callsBad() {
  var b = new borrowed C();
  {
    var own = new owned C();
    b.setCbad(own.borrow());
  }
  writeln(b.x);
}
callsBad();


