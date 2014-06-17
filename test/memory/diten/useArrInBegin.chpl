proc f2(A: []) {
  var _value = A._value;
  begin {
    use Time;
    sleep(1);
    var value = A._value;
    A[1] = 1;
  }
}

proc f1() {
  var A: [1..5] int;
  var _value = A._value;
  f2(A);
}

proc main {
  f1();
}
