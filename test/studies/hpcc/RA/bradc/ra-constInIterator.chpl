type indexType = uint(64);

const POLY2: indexType = 0x7;

iter testit(): indexType {
  const POLY: indexType = 0x7;

  yield POLY;
}


writeln(POLY2);

for i in testit() {
  writeln(i);
}
