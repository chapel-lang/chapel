use driver_domains;

var AA1DA: [1..3] [Dom1D] [1..3] real;

for i in 1..3 {
  for e in AA1DA(i) do
    e = (next(), next(), next());
}

for i in 1..3 do
  for j in Dom1D do
    writeln(AA1DA(i)(j));
