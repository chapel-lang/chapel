use ReplicatedDist;

const D = {1..3, 1..3} dmapped Replicated();

var A: [D] real;

for loc in Locales do
  on loc do
    A = here.id;

for loc in Locales do
  on loc do
    writeln(A);
writeln();

for loc in Locales do
  on loc do
    writeln(A[2.., ..2]);
writeln();

for loc in Locales do
  on loc do
    writeln(A[2, ..]);
writeln();

for loc in Locales do
  on loc do
    writeln(A.reindex(0..2, 2..4));
writeln();

writeEm(A);
writeEm(A[2.., ..2]);
writeEm(A[2, ..]);
writeEm(A.reindex(0..2, 2..4));

proc writeEm(X) {
  for loc in Locales do
    on loc do
      writeln(X);
  writeln();
}
