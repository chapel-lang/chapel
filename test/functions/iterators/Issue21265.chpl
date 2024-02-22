use List;

config param case = 1;

proc test1() {
  iter foo() {}
  var x = foo();
}

proc test2() {
  var lst: list(nothing);
}

proc test3() {
  var x: [0..3] nothing;
  writeln(x);
}

proc main() {
  if case == 1 then test1();
  if case == 2 then test2();
  if case == 3 then test3();
}
