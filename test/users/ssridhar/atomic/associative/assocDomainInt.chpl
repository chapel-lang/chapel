config const n = 10;

var D: domain(int);

var A: [D] int;

forall i in 1..n {
  atomic D.add(i);
}

writeln("Domain (after add) = ", D);

forall i in 1..n {
  atomic D.remove(i);
}

writeln("Domain (after remove) = ", D);


