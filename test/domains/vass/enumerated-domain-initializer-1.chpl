enum a {b,c,d};
var d1: domain(a) = (a.b, a.d);
writeln(d1);
d1.clear();
writeln(d1);

var d2: subdomain(d1) = (a.c,a.d);
writeln(d2);
