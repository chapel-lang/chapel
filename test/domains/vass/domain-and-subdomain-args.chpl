def testSubDomain(d: domain(?dim), sd: subdomain(d)) { }
def testSparseSubDomain(d: domain(?dim), sd: sparse subdomain(d)) { }

var d: domain(1);
var sd: subdomain(d);
var ssd: sparse subdomain(d);

testSubDomain(d, sd);
testSparseSubDomain(d, ssd);
