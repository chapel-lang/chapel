use Memory;

var u = memoryUsed();

var x = 2;
var y = 3;
var z = x + y;
writeln(z);

var uu = memoryUsed();

writeln(uu-u);
