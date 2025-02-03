use Set;

config const verbose = false;

var LocalSet= new set(int,parSafe = true);
LocalSet.add(1);
LocalSet.add(2);
LocalSet.add(3);
LocalSet.add(4);
LocalSet.add(5);

var A: [0..3] int;
writeln(A.size, " ", LocalSet.size);

// This should fail to zip, the set is longer than the array.  But it doesn't,
// improvements to leader/follower (#11505) would make it easier to detect
forall (a,b) in zip(A,LocalSet) {
    a=b;
    if verbose then writeln(b);
}
writeln(A);
