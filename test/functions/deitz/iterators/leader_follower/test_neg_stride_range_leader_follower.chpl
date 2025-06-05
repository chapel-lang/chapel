import Sort;
var D: domain((int,int), parSafe=false);

var lock: sync bool;

forall (i,j) in zip(1..7 by -1, 0..) with (ref D) {
  lock.writeEF(true);
  D += (i,j);
  lock.readFE();
}

writeln(Sort.sorted(D));
D.clear();

forall (i,j) in zip(1..7 by -2, 0..) with (ref D) {
  lock.writeEF(true);
  D += (i,j);
  lock.readFE();
}

writeln(Sort.sorted(D));
D.clear();

forall (i,j) in zip(1..4, 1..7 by -2) with (ref D) {
  lock.writeEF(true);
  D += (i,j);
  lock.readFE();
}

writeln(Sort.sorted(D));
D.clear();

forall (i,j) in zip({1..7 by -1}, 0..) with (ref D) {
  lock.writeEF(true);
  D += (i,j);
  lock.readFE();
}

writeln(Sort.sorted(D));
D.clear();

forall (i,j) in zip({1..7 by -2}, 0..) with (ref D) {
  lock.writeEF(true);
  D += (i,j);
  lock.readFE();
}

writeln(Sort.sorted(D));
D.clear();

forall (i,j) in zip({1..4}, {1..7 by -2}) with (ref D) {
  lock.writeEF(true);
  D += (i,j);
  lock.readFE();
}

writeln(Sort.sorted(D));
