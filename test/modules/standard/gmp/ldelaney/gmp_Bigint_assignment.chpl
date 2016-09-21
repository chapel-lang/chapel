use BigInteger;

// tests the assignment functions
var a = new bigint(0);    // a = 0
var b = new bigint(2);    // b = 2

writeln(a, " ", b);
a.set(b);
writeln(a, " ", b);

a.set(10);
writeln(a);

a.set(-11);
writeln(a);

a.set(-20);
writeln(a);

a.set(15.6);
writeln(a);

a.set("10010", 2);
writeln(a);

a.set("-10010");
writeln(a);

a.swap(b);
writeln(a, " ", b);

var x = a.get_ui();

if isUint(x) then writeln("uint ", x);

var y = b.get_si();
if isInt(y) then writeln("int ", y);

var z = a.get_d();
if isReal(z) then writeln("real ", z);

a.set(101);

var str = a.get_str();

if isString(str) then writeln("string ", str);
