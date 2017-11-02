config const seed: int(64) = 23;
config const numAdds = 1000;
config const numRemoves = numAdds;

use Random;
var elems: [1..numAdds] int = 1..numAdds;
Random.shuffle(elems, seed);

var inserted: [1..numAdds] bool;

const denseD = {1..numAdds*numAdds};
var D: sparse subdomain(denseD);

var removeOrder: [1..numAdds] int = 1..numAdds;
Random.shuffle(removeOrder, -seed);

test(true);
test(false);

proc test(param doSerial : bool) {
  const kind = if doSerial then "serial" else "parallel";
  writeln("Testing ", kind);
  writeln("Starting. D = ", D);
  sync serial doSerial {
    writeln("Start adding..");
    begin with (ref D) forall i in 1..numAdds with (ref D) {
      D += elems[i];
      inserted[i] = true;
    }

    writeln("Start removing..");
    var totalRemoved: sync int = 0;
    begin with (ref D) while (totalRemoved.readXX() != numRemoves) {
      forall i in 1..min(numAdds,numRemoves) with (ref D) {
        const ri = removeOrder[i];
        if inserted[ri] == true {
          D -= elems[ri];
          inserted[ri] = false;
          begin totalRemoved = totalRemoved + 1;
        }
      }
    }
  }

  writeln("Done. D = ", D);
  writeln();
}
