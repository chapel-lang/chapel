config const one = 1: uint, n = 10: uint;

var D = {1..n};
var D2 = {1..n, 1..n};

var SD: sparse subdomain(D);
var SD2: sparse subdomain(D2);


SD += n/2;
SD2 += (n/2, n/2);

writeln(SD);
writeln(SD2);
