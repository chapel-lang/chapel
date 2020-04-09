config var n = 10;

const D = {1..n, 1..n};

var DSps: sparse subdomain(D) = [i in 1..n] (i,i);

writeln("DSps is: ", DSps);

DSps = [i in 1..n] (i,1);

writeln("DSps is: ", DSps);

DSps = [i in 1..n] (1,i);

writeln("DSps is: ", DSps);

