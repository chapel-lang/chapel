var s$: sync bool;

proc f2(A: []) {
  var _value = A._value;
  begin {
    s$; // wait until after f2 returns
    var value = A._value;
    A[1] = 1;
  }
}

proc f1() {
  var A: [1..5] int;
  var _value = A._value;
  f2(A);
  s$ = true;
}

proc main {
  f1();
}
