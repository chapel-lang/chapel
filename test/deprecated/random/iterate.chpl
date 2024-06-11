use Random;

var rs = new randomStream(int, 1111),
    a: [1..10] int;

for (x, r) in zip(a, rs.iterate(a.domain)) do x = r;
writeln(a);

for (r, x) in zip(rs.iterate(a.domain), a) do x = r;
writeln(a);
