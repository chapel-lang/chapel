var int1:int = min(int);
var int2:int = 0;
var int3:int = 1;
var int4:int = max(int);

var uint1:uint = min(uint); // aka 0
var uint2:uint = 0;
var uint3:uint = 1;
var uint4:uint = max(uint);

proc testmin(x, y) {
  write("min(", x, ":", x.type:string, ", ", y, ":", y.type:string, ") = ");
  var mn = min(x, y);
  writeln(mn, " : ", mn.type:string);
}

proc testmax(x, y) {
  write("max(", x, ":", x.type:string, ", ", y, ":", y.type:string, ") = ");
  var mx = max(x, y);
  writeln(mx, " : ", mx.type:string);
}

proc test(x,y) {
  testmin(x,y);
}

// make sure nothing is too messed up
testmin(int2, int3);
testmin(uint2, uint3);
testmax(int2, int3);
testmax(uint2, uint3);
writeln();

// test all combinations of int/uint
// TODO: include different int/uint sizes
testmin(int1, uint1);
testmin(int1, uint2);
testmin(int1, uint3);
testmin(int1, uint4);
writeln();

testmin(int2, uint1);
testmin(int2, uint2);
testmin(int2, uint3);
testmin(int2, uint4);
writeln();

testmin(int3, uint1);
testmin(int3, uint2);
testmin(int3, uint3);
testmin(int3, uint4);
writeln();

testmin(int4, uint1);
testmin(int4, uint2);
testmin(int4, uint3);
testmin(int4, uint4);
writeln();
writeln();

testmax(int1, uint1);
testmax(int1, uint2);
testmax(int1, uint3);
testmax(int1, uint4);
writeln();

testmax(int2, uint1);
testmax(int2, uint2);
testmax(int2, uint3);
testmax(int2, uint4);
writeln();

testmax(int3, uint1);
testmax(int3, uint2);
testmax(int3, uint3);
testmax(int3, uint4);
writeln();

testmax(int4, uint1);
testmax(int4, uint2);
testmax(int4, uint3);
testmax(int4, uint4);
writeln();
