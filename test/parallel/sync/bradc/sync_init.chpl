var x: sync int = 1;
var y: sync int;

var z: int;

z = x;
writeln("z is: ", z);

y = z;
z = y;
writeln("z is: ", z);
