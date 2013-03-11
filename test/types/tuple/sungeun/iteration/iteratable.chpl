iter i1() {
  yield 1;
  yield 2;
  yield 3;
  yield 4;
}

iter i2() {
  yield 4;
  yield 3;
  yield 2;
  yield 1;
}

for ij in (i1(), i2()) do
  writeln(ij);

