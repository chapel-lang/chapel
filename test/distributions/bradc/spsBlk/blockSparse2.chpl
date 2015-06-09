use BlockDist;

config const n =8;

const Elems = {0..#n, 0..#n} dmapped Block({0..#n, 0..#n});
var MatElems: sparse subdomain(Elems);

for i in 0..#n {
  MatElems += (i,i);
  MatElems += (i,n-i-1);
}
//writeln("MatElems = ", MatElems);

var Dns: [Elems] int;

forall i in MatElems do
  Dns[i] = here.id + 1;

writeln("Dns is:\n", Dns);
/*
var Sps: [MatElems] int;

forall i in MatElems do
  Sps[i] = here.id + 1;

writeln("Sps is: ", Sps);
*/