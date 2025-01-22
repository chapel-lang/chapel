use Set;

config const verbose = false;

var LocalSet= new set(int,parSafe = true);
LocalSet.add(1);
LocalSet.add(2);
LocalSet.add(3);
LocalSet.add(4);
LocalSet.add(5);

var A : [0..4] int;
writeln(A.size, " ", LocalSet.size);
forall (b, a) in zip(LocalSet.contents(LocalSet.size), A) {
    a=b;
    if verbose then writeln(b);
}
writeln(A);
