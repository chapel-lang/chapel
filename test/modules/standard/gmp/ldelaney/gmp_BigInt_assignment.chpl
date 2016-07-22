use GMP;

// tests the assignment functions

var a = new BigInt(0);    // a = 0
var b = new BigInt(2);      // b = 2

writeln(a, " ", b);
a.set(b);
writeln(a, " ", b);
a.set_ui(10);
writeln(a);
a.set_si(-11);
writeln(a);
a.set(-20);
writeln(a);
a.set_d(15.6);
writeln(a);
a.set_str("10010", 2);
writeln(a);
a.set_str("-10010");
writeln(a);
a.swap(b);
writeln(a, " ", b);
var x = a.get_ui();
if isUint(x) then writeln("uint ", x);
var y = b.get_si();
if isInt(y) then writeln("int ", y);
var z = a.get_d();
if isReal(z) then writeln("real ", z);

/*
  TODO: get_d_2exp throws "incompatible pointer" warnings
    due to the tmp arg in mpz_get_d_2exp having its type ignored, and the 
    compiler making an INT(64) to use instead, leading to the wrong int size
    this should never cause a serious problem since the compiler's choice is
    larger than the needed type
    on gnu can use --ccflags -Wno-incompatible-pointer-types to squash them
  a.set(243);
  var tup = a.get_d_2exp();
  writeln(tup);
*/

a.set(101);
var str = a.get_str();
if isString(str) then writeln("string ", str);
