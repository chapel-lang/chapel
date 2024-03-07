use driver_domains;

var AA1D: [Dom1D] [1..3] real;

for e in AA1D do
  e = (next(), next(), next());

for i in Dom1D do
  writeln(AA1D(i));
