var i132 = 1:int(32);
var i164 = 1:int(64);
var i232 = 2:int(32);
var i264 = 2:int(64);
var u132 = 1:uint(32);
var u232 = 2:uint(32);

proc testminmax(x, y) {
  var minval = min(x, y);
  var maxval = max(x, y);
  writeln("min(", x, ":", x.type:string, ", ", y, ":", y.type:string, ") = ", 
          minval, ":", minval.type:string);
  writeln("max(", x, ":", x.type:string, ", ", y, ":", y.type:string, ") = ", 
          maxval, ":", maxval.type:string);
  writeln();
}

testminmax(i132, i164);
testminmax(i132, i264);
testminmax(i232, i164);

testminmax(i132, u132);
testminmax(i132, u232);
testminmax(i232, u132);

testminmax(i132, i164);
testminmax(i132, i264);
testminmax(i232, i164);

testminmax(i132, u132);
testminmax(i132, u232);
testminmax(i232, u132);

writeln (u132 + u232);

testminmax(i132, i232);
testminmax(u132, u232);
