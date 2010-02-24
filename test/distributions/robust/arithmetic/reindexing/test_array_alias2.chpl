use driver_arrays;

for e in A2D do e = next();

var AA: [101..105, 101..105] => A2D[4..8, 4..8];

AA = 0;

writeln(A2D);

AA(AA.domain.expand(-1)) = 1;

writeln(A2D);
