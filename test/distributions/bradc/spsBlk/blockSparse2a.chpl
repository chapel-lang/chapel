use BlockDist;

config const n =8;

const Elems = {0..#n} dmapped Block({0..#n});
var MatElems: sparse subdomain(Elems);

for i in 0..#n by 2 {
  MatElems += i;
}

writeln("MatElems = ", MatElems);

var Dns: [Elems] int;

forall i in MatElems do
  Dns[i] = here.id + 1;

writeln("Dns is: ", Dns);

/* TODO: Can't do I/O on sparse block arrays yet?
var Sps: [MatElems] int;

forall i in MatElems do
  Sps[i] = here.id;

writeln("Sps is: ", Sps);
*/