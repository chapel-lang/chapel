var A : [1..3,1..3] real = [(i,j) in {1..3,1..3}] i+j/10.0;
var subdA : sparse subdomain(A.domain) = ((1,1), (1,2), (2,1), (2,2));

writeln(A[subdA]);
