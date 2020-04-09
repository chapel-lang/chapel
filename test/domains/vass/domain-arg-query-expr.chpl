proc testDomain(d: domain(?dim)) { }
proc testSubdomain(d: subdomain(?dim)) { }

var d1: domain(1);
var d2: subdomain(2);

testDomain(d1);
testDomain(d2);
testSubdomain(d2);
