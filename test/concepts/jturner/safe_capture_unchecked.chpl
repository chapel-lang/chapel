var x = 4;

proc foo() {
  bar(x);
}

proc bar(y) {
  writeln(y);
}

foo();
