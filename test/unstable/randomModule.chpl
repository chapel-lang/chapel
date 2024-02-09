use Random;

var rs = new randomStream(int, 123);

const d = {1..10};

rs.skipToNth(21);
writeln(rs.getNth(42));

for x in rs.iterate(d)
    do write(x, " ");
writeln();

for x in rs.iterate(d, 1, 10)
    do write(x, " ");
writeln();
