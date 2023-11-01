use Random;

config const verbose = false;

var rng = new randomStream(eltType=real(32), parSafe=false);


var displace = (rng.getNext(), rng.getNext(), rng.getNext());

if verbose then
  writeln(displace);
