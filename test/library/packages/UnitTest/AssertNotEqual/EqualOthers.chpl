use UnitTest;
var sep = "=="*40;
var x1 = 1.0;
var y1 = 1;
try {
  UnitTest.assertNotEqual(x1,y1);
}
catch e {
  writeln(e);
  writeln(sep);
}
var x2 = 1..6;
var y2 = 1..6;
try {
  UnitTest.assertNotEqual(x2,y2);
}
catch e {
  writeln(e);
  writeln(sep);
}
var x3 = "equal string";
var y3 = "equal string";
try {
  UnitTest.assertNotEqual(x3,y3);
}
catch e {
  writeln(e);
  writeln(sep);
}