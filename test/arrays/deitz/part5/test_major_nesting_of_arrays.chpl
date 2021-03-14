var D = {1..2, 1..3, 1..4, 1..5, 1..6};

var A: [1..2] [1..3] [1..4] [1..5] [1..6] int;

for (i,j,k,l,m) in D do
  A(i)(j)(k)(l)(m) = i*10**4+j*10**3+k*10**2+l*10+m;

var B: [D] int;

for (i,j,k,l,m) in D do
  B(i,j,k,l,m) = i*10**4+j*10**3+k*10**2+l*10+m;

for (i,j,k,l,m) in D do
  if A(i)(j)(k)(l)(m) != B(i,j,k,l,m) then
    halt("unexpected inequality");

writeln(A);
writeln(B);
