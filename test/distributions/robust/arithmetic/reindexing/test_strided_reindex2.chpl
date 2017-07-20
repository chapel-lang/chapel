use driver;

const D: domain(1,int) dmapped Dist1D = {1..10};

var A: [D] int;

ref AA = A(5..10).reindex(1..11 by 2);

forall i in D do A(i) = 1;

forall e in AA do e = 0;

writeln(A);
writeln(AA);

writeln(AA.domain);
