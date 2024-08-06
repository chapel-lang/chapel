config const n = 10,
             gpuSource = true,
             gpuDest = true;
config param twoDimensional = false;

const D = if twoDimensional then {1..n, 1..n} else {1..n};
const OuterD = D.expand(1);

on (if gpuDest then here.gpus[0] else here) var Dest: [OuterD] int;
on (if gpuSource then here.gpus[0] else here) var Source: [D] int;

Source = 1;
Dest[D] = Source;
writeln(Dest);
