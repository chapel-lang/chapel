var A : [ 1..5 by -1 ] int = [ 1, 2, 3, 4, 5 ];

writeln("A[1] - A[5]: ", A[1], " ", A[2], " ", A[3], " ", A[4], " ", A[5]);
writeln("A          : ", A);

writeln("domain first: ", A.domain.first, " last: ", A.domain.last);
writeln("array first: " ,A.first, " last: ", A.last);
write(  "for  : ");
for a in A do write(a, " ");
writeln();

writeln("A: string: ", A:string);
writeln("join :     ", " ".join(A:string));
