proc testit(x) {
  writeln("For type ", x.type:string);
  var r1 = !x;
  writeln("r1 is: ", r1, ", type = ", r1.type:string);
  var r2 = ~x;
  writeln("r2 is: ", r2, ", type = ", r2.type:string);
  var r3 = x & x;
  writeln("r3 is: ", r3, ", type = ", r3.type:string);
  var r4 = x | x;
  writeln("r4 is: ", r4, ", type = ", r4.type:string);
  var r5 = x ^ x;
  writeln("r5 is: ", r5, ", type = ", r5.type:string);
  var r6 = (x == x);
  writeln("r6 is: ", r6, ", type = ", r6.type:string);
  var r7 = (x != x);
  writeln("r7 is: ", r7, ", type = ", r7.type:string);
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
