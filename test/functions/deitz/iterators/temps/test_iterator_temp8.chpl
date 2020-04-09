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

for ij in zip((1,2), (1,2), (1, 2)) do
  writeln(ij);

for ij in zip((i1(), i2()), i1()) do
  writeln(ij);
