use Random;

config const verbose = false;

var rng = PCGRandomStream.create(eltType=real(32));


var displace = (rng.getNext(), rng.getNext(), rng.getNext());

if verbose then
  writeln(displace);
