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

const domains = ({1..10, 0..5}, {1..10, 0..5 by 2}, {1..10, 0..5 by 2 align 2});
for d in domains {
    const arr = [idx in d] idx[0]:real + idx[1]:real / 10.0;
    writeln(rs.permute(arr));
    writeln("----");
    writeln(permute(arr, 314159));
    writeln("----");
    writeln(rs.permute(d));
    writeln("----");
    writeln(permute(d, 314159));
    writeln("-----------");
}
