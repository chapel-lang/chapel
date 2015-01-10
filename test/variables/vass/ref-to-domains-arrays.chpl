
proc test1() {
  var D1 = {1..4};
  ref E1 = D1;
  writeln(E1);
}

proc test2() {
  var A2: [1..4] int;
  ref B2 = A2;
  writeln(B2);
}

proc test3() {
  var A3: [1..4] int;
  ref B3 = A3[2..3];
  writeln(B3);
}

proc main() {
  test1();
  test2();
  test3();
}
