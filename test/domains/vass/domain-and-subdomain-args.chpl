proc testSubdomain(d: domain, ref sd: subdomain(d)) {
  sd = d;
}
proc testSparseSubdomain(d: domain, ref sd: sparse subdomain(d)) {
  sd = d;
}

var c: domain(1) = {1..100};
var d: domain(1) = {1..50};

var sd: subdomain(d);
var ssd: sparse subdomain(d);

testSubdomain(c, sd);
//this is correctly(?) rejected by the compiler at present:
// testSubdomain(c, ssd);
testSparseSubdomain(c, ssd);
