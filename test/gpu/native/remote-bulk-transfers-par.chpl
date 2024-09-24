config const n = parallelAssignThreshold * 2,
             gpuSource = true,
             gpuDest = true;

const D = {1..n}; // Always 1D because only simple transfers auto-parallize
const OuterD = D.expand(1);

on (if gpuDest then here.gpus[0] else here) var Dest: [OuterD] int;
on (if gpuSource then here.gpus[0] else here) var Source: [D] int;

Source = 1;
Dest[D] = Source;
writeln((+ reduce Dest) == parallelAssignThreshold * 2);
