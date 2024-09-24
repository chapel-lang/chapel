use Random;

config type StreamType=int;

var rs = new randomStream(StreamType, seed=12345);
const ranges = (1..10, 1..10 by 2, 1..10 by 2 align 2);

for r in ranges {
  const d = {r};
  const a = [i in d] i;

  writeln(rs.choose(r));
  writeln(rs.choose(d));
  writeln(rs.choose(a));
}

const domains = ({1..10, 0..5}, {1..10, 0..5 by 2}, {1..10, 0..5 by 2 align 2});

for d in domains {
  const a = [idx in d] idx[0]:real + idx[1]:real / 10.0;

  writeln(rs.choose(d));
  writeln(rs.choose(a));
}
