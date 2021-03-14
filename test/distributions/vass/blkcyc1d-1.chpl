use BlockCycDist;

var d1: domain(1) dmapped BlockCyclic(startIdx=1, blocksize=5) = {1..5};
var d2: domain(1) dmapped BlockCyclic(startIdx=(2,), blocksize=(6,)) = {2..7};

writeln(d1);
writeln(d2);
