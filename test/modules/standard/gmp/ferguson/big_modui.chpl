use GMP;

var x = new BigInt(20);
var y = new BigInt(0);

y.mod_ui(x, 11);

writeln(y);
