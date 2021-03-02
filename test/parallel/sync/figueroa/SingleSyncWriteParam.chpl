use Time;

var x: sync int = 1;
var y: single int;
var z: sync int;

z.writeEF(x.readFE());
writeln("z is ", z.readFE(),
        " and x is ", if x.isFull then "full" else "empty");

begin {sleep(6); y.writeEF(13);}
z.writeEF(y.readFF());
writeln("z is now ", z.readFF());
