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

proc test3() {
  type T = (int, (shared C?, shared C?));
  writeln(isDefaultInitializable(T));
}
test3();

proc test4() {
  type T = (int, (shared C, shared C));
  writeln(isDefaultInitializable(T));
}
test4();

