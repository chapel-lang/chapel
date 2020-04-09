use driver_arrays;

for e in A2D do e = next();

ref AA = A2D.reindex(Space2.translate(2, 2));

A2D(Space2.expand(-1)) = 0;

writeln(AA);
