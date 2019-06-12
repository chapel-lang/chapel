use UnitTest;
var sep = "=="*40;
var x1 : [1..5,1..7,1..6] real;
var y1 : [{1..5,1..7}] real;
try {
  UnitTest.assertEqual(x1,y1);
}
catch e {
  writeln(e);
  writeln(sep);
}
var x2 : [1..5,1..7] real;
var y2 : [{1..5,1..7}] int;
try {
  UnitTest.assertEqual(x2,y2);
}
catch e {
  writeln(e);
  writeln(sep);
}