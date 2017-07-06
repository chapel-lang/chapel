use driver_domains;

const D: domain(1,int,true) dmapped Dist1D = Space1;

var A: [D] int;

ref AA = A[49..54].reindex({49..55 by 2});

AA = 1;
writeln(A[45..55]);
