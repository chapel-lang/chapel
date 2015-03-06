use Time;

var x: sync int = 1,
    y, z1, z2: single int;

z1.writeEF(x);
writeln("z1 is ", z1.readFF(),
        " and x is ", if x.isFull then "full" else "empty");

begin {sleep(6); y = 13;}
z2.writeEF(y);
writeln("z2 is ", z2.readFF());
