use UnitTest;
var sep = "=="*40;

var x1 = true;
var y1 = false;
try {
  UnitTest.assertEqual(x1,y1);
}
catch e {
  writeln(e);
  writeln(sep);
}

var x2 = 1.5;
var y2 = 1.55;
try {
  UnitTest.assertEqual(x2,y2);
}
catch e {
  writeln(e);
  writeln(sep);
}

var x = "unequalstring";
var y = "unequalString";
try {
  UnitTest.assertEqual(x,y);
}
catch e {
  writeln(e);
  writeln(sep);
}