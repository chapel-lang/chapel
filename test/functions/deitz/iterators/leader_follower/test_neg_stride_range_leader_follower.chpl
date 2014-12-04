var D: domain((int,int), parSafe=false);

var lock: sync bool;

forall (i,j) in zip(1..7 by -1, 0..) with (ref D) {
  lock = true;
  D += (i,j);
  lock;
}

writeln(D.sorted());
D.clear();

forall (i,j) in zip(1..7 by -2, 0..) with (ref D) {
  lock = true;
  D += (i,j);
  lock;
}

writeln(D.sorted());
D.clear();

forall (i,j) in zip(1..4, 1..7 by -2) with (ref D) {
  lock = true;
  D += (i,j);
  lock;
}

writeln(D.sorted());
D.clear();

forall (i,j) in zip({1..7 by -1}, 0..) with (ref D) {
  lock = true;
  D += (i,j);
  lock;
}

writeln(D.sorted());
D.clear();

forall (i,j) in zip({1..7 by -2}, 0..) with (ref D) {
  lock = true;
  D += (i,j);
  lock;
}

writeln(D.sorted());
D.clear();

forall (i,j) in zip({1..4}, {1..7 by -2}) with (ref D) {
  lock = true;
  D += (i,j);
  lock;
}

writeln(D.sorted());
