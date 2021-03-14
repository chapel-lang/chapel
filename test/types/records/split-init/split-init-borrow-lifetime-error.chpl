class C {
  var intField: int;
}
record R {
  var classField: borrowed C;
}
record RQ {
  var classField: borrowed C?;
}

proc testCborrowedCQassign() {
  var c:borrowed C? = nil;
  {
    var inner = new owned C(1);
    c = inner.borrow();
  }
  writeln(c);
}
testCborrowedCQassign();

proc testCborrowedC() {
  var c:borrowed C;
  {
    var inner = new owned C(1);
    c = inner.borrow();
  }
  writeln(c);
}
testCborrowedC();

proc testCborrowedCQ() {
  var c:borrowed C?;
  {
    var inner = new owned C(1);
    c = inner.borrow();
  }
  writeln(c);
}
testCborrowedCQ();

proc testCNoType() {
  var c;
  {
    var inner = new owned C(1);
    c = inner.borrow();
  }
  writeln(c);
}
testCNoType();

proc testRassign() {
  var outer = new owned C(0);
  var x: R = new R(outer.borrow());
  {
    var myC = new owned C(1);
    x = new R(myC.borrow());
  }
  writeln(x);
}
testRassign();

proc testRQassign() {
  var x: RQ = new RQ(nil);
  {
    var myC = new owned C(1);
    x = new RQ(myC.borrow());
  }
  writeln(x);
}
testRQassign();

proc testRsplitType() {
  var x: R;
  {
    var myC = new owned C(1);
    x = new R(myC.borrow());
  }
  writeln(x);
}
testRsplitType();

proc testRQsplitType() {
  var x: RQ;
  {
    var myC = new owned C(1);
    x = new RQ(myC.borrow());
  }
  writeln(x);
}
testRQsplitType();

proc testRsplitNoType() {
  var x;
  {
    var myC = new owned C(1);
    x = new R(myC.borrow());
  }
  writeln(x);
}
testRsplitNoType();

proc testRQsplitNoType() {
  var x;
  {
    var myC = new owned C(1);
    x = new RQ(myC.borrow());
  }
  writeln(x);
}
testRQsplitNoType();
