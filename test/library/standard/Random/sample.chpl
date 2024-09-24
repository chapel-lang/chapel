use Random;

config type StreamType = int;
config const replace = false;

var rs = new randomStream(StreamType, seed=12345);
const ranges = (1..10, 1..10 by 2, 1..10 by 2 align 2);


for r in ranges {
  const d = {r};
  const a = [i in d] i;

  for n in 1..(if replace then 2*r.size else r.size) do {
    writeln(rs.sample(r, n, replace));
    writeln(rs.sample(d, n, replace));
    writeln(rs.sample(a, n, replace));
  }
}

const domains = ({1..10, 0..5}, {1..10, 0..5 by 2}, {1..10, 0..5 by 2 align 2});

for d in domains {
  const a = [idx in d] idx[0]:real + idx[1]:real / 10.0;

  for n in 1..(if replace then 2*d.size else d.size) do {
    writeln(rs.sample(d, n, replace));
    writeln(rs.sample(a, n, replace));
  }
}

// cannot sample more than the size of the range without replacement
try {
  rs.sample(1..10, 11);
} catch e: IllegalArgumentError {
  writeln("Correct Error: ", e);
} catch e {
  writeln("Wrong Error: ", e);
}

// cannot sample zero elements
try {
  rs.sample(1..10, 0, replace);
} catch e: IllegalArgumentError {
  writeln("Correct Error: ", e);
} catch e {
  writeln("Wrong Error: ", e);
}

// cannot sample from an empty range
try {
  writeln(rs.sample(1..0, 1, replace));
} catch e: IllegalArgumentError {
  writeln("Correct Error: ", e);
} catch e {
  writeln("Wrong Error: ", e);
}
