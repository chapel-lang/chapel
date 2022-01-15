config const debug = true;

const D = {1..10, 1..10};

if debug || D.dim(0) < 80 then  // should be D.dim(0).size
  writeln("small");
else
  writeln("big");
