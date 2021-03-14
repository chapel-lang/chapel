config param parSafe = true;
config const doSerial = false;

config const seed: int(64) = 23;
config const numAdds = 1000000;
config const numRemoves = numAdds;
config const numSwaps = 10;
config const stride = 7;
config const numSwapPasses = 3;

use Random;
var elems: [1..numAdds] real;
fillRandom(elems, seed);

var inserted: [1..numAdds] bool;

var D: domain(real, parSafe=parSafe);

var removeOrder: [1..numAdds] int;
[i in 1..numAdds] removeOrder[i] = i;
// permute the order in which we remove elements
for p in 1..numSwapPasses {
  for i in p..stride*numSwaps+p by stride {
    removeOrder[i%numAdds] <=> removeOrder[numAdds-i%numAdds];
  }
}

writeln(D.sorted());
sync serial doSerial || (!doSerial && !parSafe) {
  writeln("Start adding..");
  begin with (ref D) forall i in 1..numAdds with (ref D) {
    D += elems[i];
    inserted[i] = true;
  }

  writeln("Start removing..");
  var totalRemoved: sync int = 0;
  sync {
    begin with (ref D) while (totalRemoved.readXX() != numRemoves) {
      forall i in 1..min(numAdds,numRemoves) with (ref D) {
        if inserted[removeOrder[i]] == true {
          D -= elems[removeOrder[i]];
          inserted[removeOrder[i]] = false;
          begin totalRemoved.writeEF(totalRemoved.readFE() + 1);
        }
      }
    }
  }
}

writeln(D.sorted());
