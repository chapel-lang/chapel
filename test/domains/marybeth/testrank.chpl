var x : [1..10] int;
var A : [1..5,1..5] int;

dostuff(x);
dostuff(A);

proc dostuff(x: [?D] int) {
writeln("rank of domain = ", D.rank);
for i in 1..D.rank do
  writeln("length of domain(",i,") = ",D.dim(i).size);
}
