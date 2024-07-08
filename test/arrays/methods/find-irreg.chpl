enum color {red, green, blue};
use color;

var A: [false..true] real = [1.2, 3.4];
var B: [red..blue] real = [5.6, 7.8, 9.0];
var C = [red=>"RED", green=>"GREEN", blue=>"BLUE"];
var bi = false;
var ei = green;

testit(A, 3.4, bi);
testit(A, 5.6, bi);
testit(A, 1.2, bi);

testit(B, 9.0, ei);
testit(B, 10.11, ei);
testit(B, 5.6, ei);
testit(B, 0.0, ei);
testit(B, 7.8, ei);

testit(C, "GREEN", ei);
testit(C, "YELLOW", ei);
testit(C, "BLUE", ei);
testit(C, "VIOLET", ei);
testit(C, "RED", ei);

proc testit(arr, val, in idx) {
  writeln("Looking for ", val, " in ", arr);
  if arr.find(val, idx) then writeln("Found it at ", idx);
                        else writeln("Didn't find it");
}

