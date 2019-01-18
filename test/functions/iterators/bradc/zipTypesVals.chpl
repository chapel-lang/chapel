config const n = 4;

for i in zip(1..n, 0..n-1) do
  writeln(i);
writeln();

for i in zip(1..n) do
  writeln(i);
writeln();

record R {
}

iter type R.these() {
  yield 1;
  yield 2;
  yield 4;
  yield 8;
}

for i in R do
  writeln(i);

writeln();

for i in zip(R, 1..4) do
  writeln(i);

writeln();

for i in zip(1..4, R) do
  writeln(i);
