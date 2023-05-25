proc foo param { return 0; }

proc test() {
  param x = foo();
  writeln(x);
}
test();

