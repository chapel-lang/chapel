use Random;

config type st = real;

var rs = new randomStream(st, seed=12345);

const ranges = (1..10, 1..20 by 2, 1..20 by 2 align 2),
      weights = [8, 6, 1, 1, 1, 1, 1, 1, 3, 4],
      weightsT = weights: st;

for r in ranges {
    const d = {r};
    const a = [i in d] i;

    for n in 1..10 {
        writeln(rs.sample(r, n, weightsT));
        writeln(rs.sample(d, n, weightsT));
        writeln(rs.sample(a, n, weightsT));
    }
}

const seed = 314159,
      weightsR = [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0];

for r in ranges {
    const d = {r};
    const a = [i in d] i;

    for n in 1..10 {
        writeln(sample(r, n, weightsR, seed=seed));
        writeln(sample(d, n, weightsR, seed=seed));
        writeln(sample(a, n, weightsR, seed=seed));
    }
}

writeln(rs.sample(ranges[0], 50, weightsT, withReplacement=true));
writeln(rs.sample(ranges[1], 50, weightsT, withReplacement=true));
writeln(rs.sample(ranges[2], 50, weightsT, withReplacement=true));

writeln(sample(ranges[0], 50, weightsR, withReplacement=true, seed));
writeln(sample(ranges[1], 50, weightsR, withReplacement=true, seed));
writeln(sample(ranges[2], 50, weightsR, withReplacement=true, seed));

// cannot sample more than the size of the range with replacement
try {
    rs.sample(ranges[0], 50, weightsT, withReplacement=false);
} catch e: IllegalArgumentError {
    writeln("Correct Error: ", e);
} catch e {
    writeln("Wrong Error: ", e);
}

// cannot sample zero elements
try {
    rs.sample(ranges[0], 0, weightsT);
} catch e: IllegalArgumentError {
    writeln("Correct Error: ", e);
} catch e {
    writeln("Wrong Error: ", e);
}

// cannot sample from an empty range:
try {
    rs.sample(1..0, 1, weightsT);
} catch e: IllegalArgumentError {
    writeln("Correct Error: ", e);
} catch e {
    writeln("Wrong Error: ", e);
}

// weights must be the same size as the range
try {
    rs.sample(ranges[0], 1, weightsT[1..]);
} catch e: IllegalArgumentError {
    writeln("Correct Error: ", e);
} catch e {
    writeln("Wrong Error: ", e);
}
