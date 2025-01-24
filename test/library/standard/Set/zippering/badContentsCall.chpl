// Test to verify that we yell when sending in a bad length

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

// Expect this to fail with our error message, the leader needs to be sent its
// own size (I know, I know, I'm open to alternative implementations)
forall (b, a) in zip(LocalSet.contents(A.size), A) {
    a=b;
    if verbose then writeln(b);
}
writeln(A);
