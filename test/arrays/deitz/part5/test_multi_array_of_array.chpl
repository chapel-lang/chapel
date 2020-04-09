var A: [1..3, 1..3] [1..3] int;
[(i,j,k) in {1..3, 1..3, 1..3}] A(i,j)(k) = i*100+j*10+k;
writeln(A);
