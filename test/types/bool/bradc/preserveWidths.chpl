proc testit(x) {
  writeln("For type ", typeToString(x.type));
  var r1 = !x;
  writeln("r1 is: ", r1, ", type = ", typeToString(r1.type));
  var r2 = ~x;
  writeln("r2 is: ", r2, ", type = ", typeToString(r2.type));
  var r3 = x & x;
  writeln("r3 is: ", r3, ", type = ", typeToString(r3.type));
  var r4 = x | x;
  writeln("r4 is: ", r4, ", type = ", typeToString(r4.type));
  var r5 = x ^ x;
  writeln("r5 is: ", r5, ", type = ", typeToString(r5.type));
  var r6 = (x == x);
  writeln("r6 is: ", r6, ", type = ", typeToString(r6.type));
  var r7 = (x != x);
  writeln("r7 is: ", r7, ", type = ", typeToString(r7.type));
  writeln();
}

var b8: bool(8) = true;
var b16: bool(16) = true;
var b32: bool(32) = true;
var b64: bool(64) = true;
var b: bool = true;

testit(b8);
testit(b16);
testit(b32);
testit(b64);
testit(b);
