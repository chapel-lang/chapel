config var n : int = 5;

var D2 : domain(2) = {1..n, 1..n};

var iD2 : index(D2);
var i2 : index(2);

iD2 = i2;

writeln("Good.");
