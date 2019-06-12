use UnitTest;
var x1 = (1,2);
var y1 = [1,2];
UnitTest.assertNotEqual(x1,y1);
var x2 = (1,2,3);
var y2 = (1,2);
UnitTest.assertNotEqual(x2,y2);