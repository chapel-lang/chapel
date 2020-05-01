config var n : int = 4;

var d : domain(2) = {1..n, 1..n};

var a : [d] int;

forall (i,j) in d do
  a(i,j) = (i-1)*n+j-1;

writeln(a);

iter rmo(d : domain(2)) : (int,int) {
  for i in d.dim(0) do
    for j in d.dim(1) do
      yield (i,j);
}

iter cmo(d : domain(2)) : (int,int) {
  for j in d.dim(1) do
    for i in d.dim(0) do
      yield (i,j);
}

for i in rmo(d) do
  writeln(a(i));

for i in cmo(d) do
  writeln(a(i));
