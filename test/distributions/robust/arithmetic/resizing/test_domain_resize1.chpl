use driver;

var Dom: domain(2) dmapped Dist2D = Space2;

var A, B: [Dom] int;

for e in A do e = next() % 9 + 1;
for e in B do e = next() % 9 + 1;

writeln(A);
writeln(B);

Dom = Space2.expand(1);

writeln(A);
writeln(B);

Dom = Space2;

writeln(A);
writeln(B);

Dom = {1..n2+2, 1..n2-2};

writeln(A);
writeln(B);

Dom = {1..n2-2, 1..n2+2};

writeln(A);
writeln(B);
