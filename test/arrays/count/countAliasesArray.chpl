var A = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
ref Alo = A#3;
ref Ahi = A#-2;


var B = [(i,j) in {1..3, 1..3}] i + j/10.0;
ref Blo = B#(1,2);
ref Bhi = B#(-2,-1);

Alo *= -1;
Ahi += 10;

Blo *= -1;
Bhi += 10;

writeln(A);
writeln(B);
