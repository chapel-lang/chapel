use Random;

config const verbose = false;

var rng = makeRandomStream(eltType=real(32), algorithm=RNG.PCG);


var displace = (rng.getNext(), rng.getNext(), rng.getNext());

if verbose then
  writeln(displace);


delete rng;

