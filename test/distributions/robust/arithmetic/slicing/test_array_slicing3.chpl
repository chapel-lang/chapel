use driver_arrays;

for e in A1D do e = next();
for e in A2D do e = next();

for i in A1D.domain do
  if i!=1 then A1D(1..1) = A1D(i..i);

writeln(A1D);

for i in A1D.domain do
  if i!=1 then A1D(i..i) = A1D(1..1);

writeln(A1D);

for (i,j) in A2D.domain do
  if !(i==1 && j==1) then A2D(1..1, 1..1) = A2D(i..i,j..j);

writeln(A2D);

for (i,j) in A2D.domain do
  if !(i==1 && j==1) then A2D(i..i, j..j) = A2D(1..1, 1..1);

writeln(A2D);
