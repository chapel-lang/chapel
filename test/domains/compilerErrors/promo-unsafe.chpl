
var ad: domain(int, parSafe=false);
ad.add([123]);  // par-unsafe if adding multiple elements
writeln(ad);

var rd = {1..1000};
var sd: sparse subdomain(rd);
sd.add([321]);  // par-unsafe if adding multiple elements
writeln(sd);
