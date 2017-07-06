// Verify that accesses to replicated arrays do not involve communication.

use ReplicatedDist;

const ls = Locales;
writeln("running on locales ", ls.domain);

var d = {1..3,1..3} dmapped Replicated(ls);
var a: [d] int;

//
// set 'a' on each locale
//
for l in Locales do
  on l do
    a = 33;

for loc in ls {
  write("on ", loc);
  on loc {
    var temp = 5;
    write("  ", temp, " -> ");
    local {
      temp = a[2,3];  // should be purely local
    }
    writeln(temp);
  }
}

writeln("done");
