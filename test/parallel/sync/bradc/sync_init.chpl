var x: sync int = 1;
var y: sync int;

var z: int;

z = x.readFE();
writeln("z is: ", z);

y = z;
z = y.readFE();
writeln("z is: ", z);
