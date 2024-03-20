use Random;

config const verbose = false;

var rng = new randomStream(eltType=real(32));


var displace = (rng.next(), rng.next(), rng.next());

if verbose then
  writeln(displace);
