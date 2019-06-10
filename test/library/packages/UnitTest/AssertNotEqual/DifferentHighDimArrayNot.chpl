use UnitTest;
var x : [1..5,1..7,1..6] real;
var y : [{1..5,1..7}] real;
UnitTest.assertNotEqual(x,y);