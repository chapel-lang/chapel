
var D: domain(2) = [1..m, 1..n];
var A: [D] real;

var SubD: subdomain(D) = D;

var (ippiv, info) = pdgetrf(A, SubD);
