var x1, x2: int;

iter foo() {
  yield x1;
  yield x2;
}

foo() = 2;

writeln(x1, x2);
