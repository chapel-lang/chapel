enum color { red, blue, green };

var D1: domain(1);
var D2: domain(1);

var DA1: domain(string);
var DA2: domain(string);

var DE1: domain(color);
var DE2: domain(color);

var DS1: sparse subdomain(D1);
var DS2: sparse subdomain(D2);

writeln(D1.dist == D1.dist);
writeln(D1.dist == D2.dist);
writeln(D1.dist == DA1.dist);
writeln(D1.dist == DE1.dist);
writeln(D1.dist == DS1.dist);

// TODO: Once this is a passing future, should
// add more tests of different equalities down
// here.
