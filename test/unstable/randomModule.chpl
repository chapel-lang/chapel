use Random;

use Random;

var rs = new randomStream(int, 123),
    a = [i in 1..10] i;

writeln(rs.choice(a, 1));
writeln(rs.choice(1..10, 1));
writeln(rs.choice(a.domain, 1));

rs.skipToNth(21);
writeln(rs.getNth(42));

for x in rs.iterate(a.domain)
    do write(x, " ");
writeln();

for x in rs.iterate(a.domain, 1, 10)
    do write(x, " ");
writeln();
