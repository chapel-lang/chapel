use Random;

config type st = real;

var rs = new randomStream(st, seed=12345);

const ranges = (1..10, 1..20 by 2, 1..20 by 2 align 2),
      weights = [8, 6, 1, 1, 1, 1, 1, 1, 3, 4],
      weightsT = weights: st;

writeln("randomStream.sample:");
for r in ranges {
    const d = {r};
    const a = [i in d] i;

    writeln("sampling from ", r);
    for n in 1..10 {
        writeln(rs.sample(r, n, weightsT));
        writeln(rs.sample(d, n, weightsT));
        writeln(rs.sample(a, n, weightsT));
    }
}

const seed = 314159,
      weightsR = [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0];

writeln("Random.sample:");
for r in ranges {
    const d = {r};
    const a = [i in d] i;

    writeln("sampling from ", r);
    for n in 1..10 {
        writeln(sample(r, n, weightsR, seed=seed));
        writeln(sample(d, n, weightsR, seed=seed));
        writeln(sample(a, n, weightsR, seed=seed));
    }
}

writeln("randomStream.sample with replacement:");
writeln(rs.sample(ranges[0], 50, weightsT, withReplacement=true));
writeln(rs.sample(ranges[1], 50, weightsT, withReplacement=true));
writeln(rs.sample(ranges[2], 50, weightsT, withReplacement=true));

writeln("Random.sample with replacement:");
writeln(sample(ranges[0], 50, weightsR, withReplacement=true, seed));
writeln(sample(ranges[1], 50, weightsR, withReplacement=true, seed));
writeln(sample(ranges[2], 50, weightsR, withReplacement=true, seed));

writeln("\nErrors:");
try {
    writeln("too many samples for (withReplacment=false):");
    rs.sample(ranges[0], 50, weightsT, withReplacement=false);
} catch e: IllegalArgumentError {
    writeln("\tCorrect Error: ", e);
} catch e {
    writeln("\tWrong Error: ", e);
}

try {
    writeln("zero samples:");
    rs.sample(ranges[0], 0, weightsT);
} catch e: IllegalArgumentError {
    writeln("\tCorrect Error: ", e);
} catch e {
    writeln("\tWrong Error: ", e);
}

try {
    writeln("empty population:");
    rs.sample(1..0, 1, weightsT);
} catch e: IllegalArgumentError {
    writeln("\tCorrect Error: ", e);
} catch e {
    writeln("\tWrong Error: ", e);
}

try {
    writeln("weights size doesn't match population size:");
    rs.sample(ranges[0], 1, weightsT[1..]);
} catch e: IllegalArgumentError {
    writeln("\tCorrect Error: ", e);
} catch e {
    writeln("\tWrong Error: ", e);
}

try {
    writeln("negative weights:");
    sample(ranges[0], 1, [5.0, 5.0, 5.0, -1.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0]);
} catch e: IllegalArgumentError {
    writeln("\tCorrect Error: ", e);
} catch e {
    writeln("\tWrong Error: ", e);
}

try {
    writeln("all zero weights:");
    sample(ranges[0], 1, [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);
} catch e: IllegalArgumentError {
    writeln("\tCorrect Error: ", e);
} catch e {
    writeln("\tWrong Error: ", e);
}
