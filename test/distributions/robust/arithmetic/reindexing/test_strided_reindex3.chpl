use driver_domains;

const D: domain(1,int,true) dmapped Dist1D = Space1;

var A: [D] int;

// We expect this range to straddle two locales
ref AA = A[48..53].reindex(1..11 by 2);

for i in AA.domain do writeln(i, ": ", AA[i].locale);
