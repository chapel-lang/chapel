
proc foo() {
  var A:[1..10] int;
  return (A[1..2], A[2..3]);
}
proc bar(x) {
  writeln(x);
  x(0)(2) = 1;
  writeln(x);
}

proc test() {
  bar(foo());
}

test();
