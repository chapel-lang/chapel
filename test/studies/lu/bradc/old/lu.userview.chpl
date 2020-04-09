
var D: domain(2) dmapped (Scalapack(10)) = [1..m, 1..n];
var A: [D] real;

var SubD: subdomain(D) = D;

var (ipiv, info) = pdgetrf(A, SubD);
