use UnitTest;
var sep = "=="*40;
// Type 1
var x1 = [1,5,7];
var y1 = [1,7,6];
try {
  UnitTest.assertEqual(x1,y1);
}
catch e {
  writeln(e);
  writeln(sep);
}
// Type 2
var x2 = [1,7,8,5,8];
var y2 = [1,7,6,4];
try {
  UnitTest.assertEqual(x2,y2);
}
catch e {
  writeln(e);
  writeln(sep);
}
var x3 = [1,7,6,5,8];
var y3 = [1,7,6];
try {
  UnitTest.assertEqual(x3,y3);
}
catch e {
  writeln(e);
  writeln(sep);
}