extern type smallCArr = 3*int(32);
extern proc initCArr(ref x: smallCArr);

var x: smallCArr;

initCArr(x);

writeln("x is: ", x);
