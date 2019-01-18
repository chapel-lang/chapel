use driver_arrays;

for e in A2D do e = next();

ref AA = A2D[4..8, 4..8].reindex(101..105, 101..105);

AA = 0;

writeln(A2D);

AA(AA.domain.expand(-1)) = 1;

writeln(A2D);
