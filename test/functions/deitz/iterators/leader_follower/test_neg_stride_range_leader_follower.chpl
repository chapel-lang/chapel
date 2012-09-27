var D: domain((int,int));

var lock: sync bool;

forall (i,j) in zip(1..7 by -1, 0..) {
  lock = true;
  D += (i,j);
  lock;
}

writeln(D.sorted());
D.clear();

forall (i,j) in zip(1..7 by -2, 0..) {
  lock = true;
  D += (i,j);
  lock;
}

writeln(D.sorted());
D.clear();

forall (i,j) in zip(1..4, 1..7 by -2) {
  lock = true;
  D += (i,j);
  lock;
}

writeln(D.sorted());
D.clear();

forall (i,j) in zip({1..7 by -1}, 0..) {
  lock = true;
  D += (i,j);
  lock;
}

writeln(D.sorted());
D.clear();

forall (i,j) in zip({1..7 by -2}, 0..) {
  lock = true;
  D += (i,j);
  lock;
}

writeln(D.sorted());
D.clear();

forall (i,j) in zip({1..4}, {1..7 by -2}) {
  lock = true;
  D += (i,j);
  lock;
}

writeln(D.sorted());
