use BlockDist;

config const n =8;

const Elems = {0..#n} dmapped Block({0..#n});
var MatElems: sparse subdomain(Elems);

for i in 0..#n by 2 {
  MatElems += i;
}

writeln("MatElems = ", MatElems);

forall i in MatElems do
  writeln(i, " is on, ", here);
