// Exercise MatrixHelpers module.

use MatrixHelpers;

var A = reshape([1.0, 2.0, 3.0, 4.0, 5.0, 6.0], {1..2, 1..3});

writeln("A");
writeln(A);
writeln("A'");
writeln(transpose(A));
writeln();

var B = reshape([7.0, 8.0, 9.0, 10.0, 11.0, 12.0], {1..3, 1..2});

writeln("B");
writeln(B);
writeln();

var C = dotProduct(A, B);

writeln("C = dotProduct(A, B)");
writeln(C);
writeln();

var D = dotProduct(B, A);

writeln("D = dotProduct(B, A)");
writeln(D);
writeln();

var E = reshape([10.0, 11.0,
                 12.0, 13.0,
                 14.0, 15.0,
                 16.0, 17.0],
                {1..4, 1..2}),
  F = reshape([18.0, 19.0, 20.0,
               21.0, 22.0, 23.0],
              {1..2, 1..3});

writeln("E");
writeln(E);
writeln();

writeln("F");
writeln(F);
writeln();

var G = dotProduct(E, F);

writeln("G = dotProduct(E, F)");
writeln(G);
writeln();
