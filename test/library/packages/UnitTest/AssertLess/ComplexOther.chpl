use UnitTest;
var sep ="=="*40;
var x = 5;
var y = 7i;
try {
  UnitTest.assertLessThan(x,y);
}
catch e {
  writeln(e);
  writeln(sep);
}
var x1 = 1 + 5i;
var y1 = 5i;
try {
  UnitTest.assertLessThan(x1,y1);
}
catch e {
  writeln(e);
  writeln(sep);
}
var y2 = (1,2,3);
try {
  UnitTest.assertLessThan(x1,y2);
}
catch e {
  writeln(e);
  writeln(sep);
}