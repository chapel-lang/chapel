use UnitTest;
var sep = "=="*40;
var x1 = ..6;
var y1 = ..7;
try {
  UnitTest.assertEqual(x1,y1);
}
catch e {
  writeln(e);
  writeln(sep);
}
var x2 = 1..6;
var y2 = 2..7;
try {
  UnitTest.assertEqual(x2,y2);
}
catch e {
  writeln(e);
  writeln(sep);
}