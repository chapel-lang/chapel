// Test reading of single vars - currently generates an error.
// See also test/types/sync/vass/readThis.chpl
use IO;

var g2: single real;
read(g2);
writeln("g2 = ", g2);
