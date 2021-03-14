// test sent in by Ariful Azad; edited slightly by Brad Chamberlain
use BlockDist;
var dDomDist1 = {0..9} dmapped Block({0..9});
var spDomDist1: sparse subdomain(dDomDist1);
spDomDist1 = (0,5,7);
writeln(  spDomDist1.size);
spDomDist1.clear();
writeln(  spDomDist1.size);
spDomDist1 = (3, 9);
writeln(  spDomDist1.size);
