var d : domain(2) = {1..3, 1..3};
var a : [d] int;

forall ij in d {
  a(ij(0),ij(1)) = (ij(0)-1)*3 + ij(1);
}

for ij in d {
  writeln(a(ij(0),ij(1)));
}
