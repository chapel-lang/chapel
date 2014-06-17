config const doSerial = true;

config const seed: int(64) = 23;
config const numAdds = 1000;
config const numRemoves = numAdds;
config const numSwaps = 10;
config const stride = 7;
config const numSwapPasses = 3;

use Random;
var relems: [1..numAdds] real;
fillRandom(relems, seed);
var elems: [1..numAdds] int = relems*numAdds;

var inserted: [1..numAdds] bool;

const denseD = {1..numAdds*numAdds};
var D: sparse subdomain(denseD);

var removeOrder: [1..numAdds] int;
[i in 1..numAdds] removeOrder[i] = i;
// permute the order in which we remove elements
for p in 1..numSwapPasses {
  for i in p..stride*numSwaps+p by stride {
    removeOrder[i%numAdds] <=> removeOrder[numAdds-i%numAdds];
  }
}

writeln(D);
sync serial doSerial {
  writeln("Start adding..");
  begin forall i in 1..numAdds {
    D += elems[i];
    inserted[i] = true;
  }

  writeln("Start removing..");
  var totalRemoved: sync int = 0;
  begin while (totalRemoved.readXX() != numRemoves) {
    forall i in 1..min(numAdds,numRemoves) {
      if inserted[removeOrder[i]] == true {
        D -= elems[removeOrder[i]];
        inserted[removeOrder[i]] = false;
        begin totalRemoved = totalRemoved + 1;
      }
    }
  }
}

writeln(D);
