use UnitTest;
var x1D = [1,2,5];
var y1D = [1,2,5];
var xND : [1..5,1..7] real;
var yND : [{1..5,1..7}] real;
UnitTest.assertEqual(x1D,y1D);
UnitTest.assertEqual(xND,yND);