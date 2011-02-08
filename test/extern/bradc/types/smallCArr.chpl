_extern type smallCArr = 3*int(32);
_extern proc initCArr(inout x: smallCArr);

var x: smallCArr;

initCArr(x);

writeln("x is: ", x);
