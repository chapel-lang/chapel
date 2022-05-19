use VectorizingIterator;

var A = [1];

for i in vectorizeOnly(1..1) do
  writeln(i);

for a in vectorizeOnly(A) do
  writeln(a);

for (i,a) in vectorizeOnly(1..1, A) do
  writeln((i,a));

forall i in vectorizeOnly(1..1) do
  writeln(i);

forall a in vectorizeOnly(A) do
  writeln(a);

forall (i,a) in vectorizeOnly(1..1, A) do
  writeln((i,a));
