class C { var x: int = 0; }

proc test1() {
  type T = (int, shared C?);
  writeln(isDefaultInitializable(T));
}
test1();

proc test2() {
  type T = (int, shared C);
  writeln(isDefaultInitializable(T));
}
test2();

