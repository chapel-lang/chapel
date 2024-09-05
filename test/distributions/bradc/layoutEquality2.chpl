use CompressedSparseLayout;

var D1: domain(2) = {1..10, 1..10};
var D2: domain(2) = {1..3, 1..3};

var DS1: sparse subdomain(D1);
var DS2: sparse subdomain(D2);

var DS_CSR1: sparse subdomain(D1) dmapped new cs();
var DS_CSR2: sparse subdomain(D2) dmapped new cs();

writeln(DS1.distribution == DS1.distribution);
writeln(DS1.distribution == DS2.distribution);
writeln(DS1.distribution == DS_CSR1.distribution);
writeln(DS1.distribution == DS_CSR2.distribution);
writeln();

writeln(DS2.distribution == DS2.distribution);
writeln(DS2.distribution == DS_CSR1.distribution);
writeln(DS2.distribution == DS_CSR2.distribution);
writeln();

writeln(DS_CSR1.distribution == DS_CSR1.distribution);
writeln(DS_CSR1.distribution == DS_CSR2.distribution);
writeln();

writeln(DS_CSR2.distribution == DS_CSR2.distribution);
writeln();


writeln(DS1.distribution != DS1.distribution);
writeln(DS1.distribution != DS2.distribution);
writeln(DS1.distribution != DS_CSR1.distribution);
writeln(DS1.distribution != DS_CSR2.distribution);
writeln();

writeln(DS2.distribution != DS2.distribution);
writeln(DS2.distribution != DS_CSR1.distribution);
writeln(DS2.distribution != DS_CSR2.distribution);
writeln();

writeln(DS_CSR1.distribution != DS_CSR1.distribution);
writeln(DS_CSR1.distribution != DS_CSR2.distribution);
writeln();

writeln(DS_CSR2.distribution != DS_CSR2.distribution);
writeln();
