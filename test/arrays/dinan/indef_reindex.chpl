var A: [0..99] real;

ref BP = A[10..19];
ref B = BP.reindex(0..);

writeln(A.domain, BP.domain, B.domain);
