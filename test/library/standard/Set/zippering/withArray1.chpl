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
forall (a,b) in zip(A,LocalSet.contents(A.size)) {
    a=b;
    if verbose then writeln(b);
}
writeln(A);
