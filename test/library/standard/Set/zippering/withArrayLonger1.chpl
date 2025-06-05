use Set;

config const verbose = false;

var LocalSet= new set(int,parSafe = true);
for i in 1..10 {
  LocalSet.add(i);
}

var A : [0..9] int;
writeln(A.size, " ", LocalSet.size);
forall (a,b) in zip(A,LocalSet) {
    a=b;
    if verbose then writeln(b);
}
writeln(LocalSet);
writeln(A);
