use Random;

var rs = new randomStream(int, 12345);
const ranges = (1..10, 1..10 by 2, 1..10 by 2 align 2);

for r in ranges {
    const arr = [i in r] i;
    writeln(rs.permute(arr));
    writeln(permute(arr, 314159));
    writeln(rs.permute(r));
    writeln(permute(r, 314159));
    writeln(rs.permute({r}));
    writeln(permute({r}, 314159));
    writeln("-----------");
}
