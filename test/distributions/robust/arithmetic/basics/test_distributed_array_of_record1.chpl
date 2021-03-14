use driver_domains;

record r {
  var r: real;
};

var AA1D: [Dom1D] r;

for e in AA1D do
  e.r = next();

for i in Dom1D do
  writeln(AA1D(i));
