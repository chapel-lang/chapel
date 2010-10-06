def testDomain(d: domain(?dim)) { }
def testSubdomain(d: subdomain(?dim)) { }

var d1: domain(1);
var d2: subdomain(2);

testDomain(d1);
testDomain(d2);
testSubdomain(d2);
