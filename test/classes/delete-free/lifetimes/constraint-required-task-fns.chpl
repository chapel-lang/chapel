class C { var x: int; }
proc noop() { }

proc setit_begin( ref lhs: C, rhs: C ) lifetime lhs < rhs {
  sync begin with (ref lhs)
    lhs = rhs;
}

proc setit_cobegin( ref lhs: C, rhs: C ) lifetime lhs < rhs {
  cobegin with (ref lhs) {
    lhs = rhs;
    noop();
  }
}

proc setit_coforall( ref lhs: C, rhs: C ) lifetime lhs < rhs {
  coforall i in 1..2 with (ref lhs) {
    if i == 1 then
      lhs = rhs;
    else
      noop();
  }
}

proc setit_forall( ref lhs: C, rhs: C ) lifetime lhs < rhs {
  forall i in 1..2 with (ref lhs) {
    if i == 1 then
      lhs = rhs;
    else
      noop();
  }
}

proc setit_on( ref lhs: C, rhs: C ) lifetime lhs < rhs {
  on Locales[numLocales-1] do
    lhs = rhs;
}


proc test_begin() {
  var a = new owned C(1);
  var b = new owned C(2);
  var ab = a.borrow();
  var bb = b.borrow();
  setit_begin(ab, bb);
}
test_begin();

proc test_cobegin() {
  var a = new owned C(1);
  var b = new owned C(2);
  var ab = a.borrow();
  var bb = b.borrow();
  setit_cobegin(ab, bb);
}
test_cobegin();

proc test_coforall() {
  var a = new owned C(1);
  var b = new owned C(2);
  var ab = a.borrow();
  var bb = b.borrow();
  setit_coforall(ab, bb);
}
test_coforall();

proc test_forall() {
  var a = new owned C(1);
  var b = new owned C(2);
  var ab = a.borrow();
  var bb = b.borrow();
  setit_forall(ab, bb);
}
test_forall();

proc test_on() {
  var a = new owned C(1);
  var b = new owned C(2);
  var ab = a.borrow();
  var bb = b.borrow();
  setit_on(ab, bb);
}
test_on();
