// Test reading of syng vars - currently generates an error.
// See also test/types/single/vass/readThis.chpl
use IO;

var g2: sync real;
read(g2);
writeln("g2 = ", g2);
