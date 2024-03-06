use Random, BlockDist;

var d = blockDist.createDomain({1..100}),
    a = [i in d] i;

writeln(sample(a, 5));
