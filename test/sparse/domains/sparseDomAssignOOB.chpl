const D1: domain(1) = {1..10};
var SD1: sparse subdomain(D1);

var inds = [2, 3, 4];

SD1 += inds;

const D2: domain(1);
var SD2: sparse subdomain(D2);
var SA2: [SD2] real;

SD2 = SD1;  // This result in an execution-time error: since D2 is
            // empty, there are no legal indices that can be put into
            // SD2.

writeln(SD1);
writeln(SD2);

SA2[2] = 1.2;  // Unfortunately, the OOB error doesn't show up until here


