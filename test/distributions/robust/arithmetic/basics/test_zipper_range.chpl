use driver_arrays;

for a in A1D do a = next();

forall (a,i) in zip(A1D, 1.. by 2) do
  a = i;

forall (i,j) in zip(Dom1D, 1.. by 2) do
  A1D(i) += j;

writeln(A1D);
