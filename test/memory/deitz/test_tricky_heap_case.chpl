use Time;

proc foo() {
  var A: [1..10] int;
  return A;
}

proc bar(A: []) {
  for e in A {
    begin {
      sleep(2);
      e = 12;
    }
  }
}

bar(foo());
