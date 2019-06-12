use UnitTest;
var sep ="=="*40;
var x = 1.0;
var y = 1;
try {
  UnitTest.assertLessThan(x,y);
}
catch e {
  writeln(e);
  writeln(sep);
}
var x1 = 1..6;
try {
  UnitTest.assertLessThan(x1,x1);
}
catch e {
  writeln(e);
  writeln(sep);
}
var y1 = "equal string";
try {
  UnitTest.assertLessThan(y1,y1);
}
catch e {
  writeln(e);
  writeln(sep);
}