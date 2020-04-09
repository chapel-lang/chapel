use LayoutCS;

var D1: domain(2) = {1..10, 1..10};
var D2: domain(2) = {1..3, 1..3};

var DS1: sparse subdomain(D1);
var DS2: sparse subdomain(D2);

var DS_CSR1: sparse subdomain(D1) dmapped CS();
var DS_CSR2: sparse subdomain(D2) dmapped CS();

writeln(DS1.dist == DS1.dist);
writeln(DS1.dist == DS2.dist);
writeln(DS1.dist == DS_CSR1.dist);
writeln(DS1.dist == DS_CSR2.dist);
writeln();

writeln(DS2.dist == DS2.dist);
writeln(DS2.dist == DS_CSR1.dist);
writeln(DS2.dist == DS_CSR2.dist);
writeln();

writeln(DS_CSR1.dist == DS_CSR1.dist);
writeln(DS_CSR1.dist == DS_CSR2.dist);
writeln();

writeln(DS_CSR2.dist == DS_CSR2.dist);
writeln();


writeln(DS1.dist != DS1.dist);
writeln(DS1.dist != DS2.dist);
writeln(DS1.dist != DS_CSR1.dist);
writeln(DS1.dist != DS_CSR2.dist);
writeln();

writeln(DS2.dist != DS2.dist);
writeln(DS2.dist != DS_CSR1.dist);
writeln(DS2.dist != DS_CSR2.dist);
writeln();

writeln(DS_CSR1.dist != DS_CSR1.dist);
writeln(DS_CSR1.dist != DS_CSR2.dist);
writeln();

writeln(DS_CSR2.dist != DS_CSR2.dist);
writeln();
