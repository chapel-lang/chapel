
var DR = {1..10};
var SS: sparse subdomain(DR);
writeln(DR.last);
writeln(SS.last); // undefined behavior; currently an error
