use UnitTest;
var x : [1..5,1..7] real;
var y : [{1..7,1..5}] int;
UnitTest.assertLessThan(x,y);