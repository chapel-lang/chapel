proc testSubdomain(d: domain, sd: subdomain(d)) { }
proc testSparseSubdomain(d: domain, sd: sparse subdomain(d)) { }

var c: domain(1);
var d: domain(1);

var sd: subdomain(d);
var ssd: sparse subdomain(d);

testSubdomain(c, sd);
//this is correctly(?) rejected by the compiler at present:
// testSubdomain(c, ssd);
testSparseSubdomain(c, ssd);
