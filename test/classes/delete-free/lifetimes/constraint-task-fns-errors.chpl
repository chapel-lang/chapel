class C { var x: int; }
proc noop() { }

proc setit_begin( ref lhs: C?, rhs: C ) lifetime lhs < rhs {
  sync begin with (ref lhs)
    lhs = rhs;
}

proc setit_cobegin( ref lhs: C?, rhs: C ) lifetime lhs < rhs {
  cobegin with (ref lhs) {
    lhs = rhs;
    noop();
  }
}

proc setit_coforall( ref lhs: C?, rhs: C ) lifetime lhs < rhs {
  coforall i in 1..2 with (ref lhs) {
    if i == 1 then
      lhs = rhs;
    else
      noop();
  }
}

proc setit_forall( ref lhs: C?, rhs: C ) lifetime lhs < rhs {
  forall i in 1..2 with (ref lhs) {
    if i == 1 then
      lhs = rhs;
    else
      noop();
  }
}

proc setit_on( ref lhs: C?, rhs: C ) lifetime lhs < rhs {
  on Locales[numLocales-1] do
    lhs = rhs;
}


proc test_begin() {
  var bb: borrowed C?;
  {
    var a = new owned C(1);
    setit_begin(bb, a.borrow());
  }
  writeln(bb!.x);
}
test_begin();

proc test_cobegin() {
  var bb: borrowed C?;
  {
    var a = new owned C(1);
    setit_cobegin(bb, a.borrow());
  }
  writeln(bb!.x);
}
test_cobegin();

proc test_coforall() {
  var bb: borrowed C?;
  {
    var a = new owned C(1);
    setit_coforall(bb, a.borrow());
  }
  writeln(bb!.x);
}
test_coforall();

proc test_forall() {
  var bb: borrowed C?;
  {
    var a = new owned C(1);
    setit_forall(bb, a.borrow());
  }
  writeln(bb!.x);
}
test_forall();

proc test_on() {
  var bb: borrowed C?;
  {
    var a = new owned C(1);
    setit_on(bb, a.borrow());
  }
  writeln(bb!.x);
}
test_on();
