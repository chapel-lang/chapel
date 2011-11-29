var D = [1..2, 3..4];
var E = [5..6, 7..8];

for ((i,j),(k,l)) in (D,E) do
  writeln((i,j,k,l));

writeln();

for ((i,j),(k,l)) in [D,E] do
  writeln((i,j,k,l));

