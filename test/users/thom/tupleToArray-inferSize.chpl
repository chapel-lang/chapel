var t1 = (1, 2, 3);
var t2 = (1.0, 2.0, 3.0);
var t3 = ("one", "two", "three");

var x1: [1..] int = t1;
var x2: [1..] real = t2;
var x3: [1..] string = t3;

writeln("x1 is: ", x1);
writeln("x2 is: ", x2);
writeln("x3 is: ", x3);
