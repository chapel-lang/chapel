use Sort;

var A = [1.4, 6.3, 2.8, 3.7, 9.6];

iter genA() {
  yield 1.4;
  yield 6.3;
  yield 2.8;
  yield 3.7;
  yield 9.6;
}

iter genA2() {
  for a in A do
    yield a;
}


writeln("A is: ", A);

for a in sorter(A) do
  write(a, " ");
writeln();

for a in sorter(genA()) do
  write(a, " ");
writeln();

for a in sorter(genA2()) do
  write(a, " ");
writeln();

writeln("A is: ", A);
