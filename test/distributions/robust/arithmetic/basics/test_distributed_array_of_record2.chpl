use driver_domains;

record r {
  var r: real;
};

var AA1DA: [1..3] [Dom1D] r;

for i in 1..3 {
  for e in AA1DA(i) do
    e.r = next();
}

for i in 1..3 do
  for j in Dom1D do
    writeln(AA1DA(i)(j));
