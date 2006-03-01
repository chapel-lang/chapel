config var n : integer = 4;
var d = [1..n, 1..n, 1..n];
var a : [d] integer;

for i,j,k in d do
  a(i,j,k) = (i-1)*n**2 + (j-1)*n + k;

for ijk in d do
  writeln(a(ijk));
