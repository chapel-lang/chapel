use driver_domains;

var A, B, C: [Dom2D] real;

A = 1.0;
B = 2.0;

C = A + B;

writeln(C);

C = A + (+ reduce A) * B;

writeln(C);
