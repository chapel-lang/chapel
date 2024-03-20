use Random;

var rs = new randomStream(int, 12345);

const r = 1..10,
      d = {r},
      a = [i in d] i;

writeln(rs.choice(r, 1));
writeln(rs.choice(d, 1));
writeln(rs.choice(a, 1));
