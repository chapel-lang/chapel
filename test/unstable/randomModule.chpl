use Random;

use Random;

var a: [1..10] int;

// overload that doesn't accept a seed is unstable
fillRandom(a);

// overload that doesn't accept a seed is unstable
shuffle(a);

permutation(a);
permutation(a, 123);
writeln(a);

var rs = new randomStream(int, 123);

rs.permutation(a);
writeln(a);

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
