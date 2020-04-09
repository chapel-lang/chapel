use Time;

var x: sync int = 1;
var y: single int;
var z: sync int;

z.writeEF(x);
writeln("z is ", z.readFE(),
        " and x is ", if x.isFull then "full" else "empty");

begin {sleep(6); y = 13;}
z.writeEF(y);
writeln("z is now ", z.readFF());
