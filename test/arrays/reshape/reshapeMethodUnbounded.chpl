var A = [111, 112;
         121, 122;
         ;
         211, 212;
         221, 222];

ref B = A.reshape(1..);
writeln(B, ": [", B.domain, "]");
B[1] = 1;

ref C = A.reshape(0..);
writeln(C, ": [", C.domain, "]");
C[1] = 2;

writeln(A);
