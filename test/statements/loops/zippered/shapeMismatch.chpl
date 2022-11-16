var r = 1..6;
var D = {1..2, 1..3};
var D2 = {1..3, 1..2};
var A: [D] real = [(i,j) in D] i + j/ 10.0;

for ijka in zip(r, D, D2, A) do
  writeln(ijka);

writeln("---");

for ijka in zip(D, r, D2, A) do
  writeln(ijka);

writeln("---");

for ijka in zip(D2, r, D, A) do
  writeln(ijka);

writeln("---");

for ijka in zip(A, r, D, D2) do
  writeln(ijka);

writeln("---");

