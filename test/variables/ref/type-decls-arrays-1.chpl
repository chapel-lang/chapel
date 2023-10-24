// When declaring the type of a 'ref' to an array, it should be an error if
// the domain of that type differs from the domain of the referenced array.


var d1, d2: domain(1);

var a1: [d1] int;
ref r1: [d1] int = a1; // OK
ref r2: [d2] int = a1; // error: d2 has a different identity than a1.domain 

// Analogously for domains.

use BlockDist;
const d3 = {1..3};
var dist1 = new blockDist(d3);
var dist2 = new blockDist(d3);
var dom1 = dist1.createDomain(d3);
var dom2 = dist2.createDomain(d3);
ref br1: dom1.type = dom1;  // OK
ref br2: dom2.type = dom1;  // error: dom2.distribution has a different identity
                            //   than dom1.distribution
