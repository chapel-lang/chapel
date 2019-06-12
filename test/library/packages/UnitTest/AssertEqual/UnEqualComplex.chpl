use UnitTest;
var sep = "=="*40;
var x1 = 5;
var y1 = 7i;
try {
  UnitTest.assertEqual(x1,y1);
}
catch e {
  writeln(e);
  writeln(sep);
}
var x2 = 5 + 6i;
var y2 = 7i;
try {
  UnitTest.assertEqual(x2,y2);
}
catch e {
  writeln(e);
  writeln(sep);
}
