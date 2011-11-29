use driver_domains;

var AA1D: [1..3] [Dom1D] real;

for i in 1..3 do
  forall e in AA1D(i) do
    e = i;

for i in 1..3 do
  writeln(AA1D(i));

writeln(dist_eq(AA1D(1).domain.dist, Dist1D));
writeln(dist_eq(AA1D(2).domain.dist, Dist1D));
writeln(dist_eq(AA1D(3).domain.dist, Dist1D));
