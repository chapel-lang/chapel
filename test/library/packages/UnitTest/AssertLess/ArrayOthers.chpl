use UnitTest;
var sep ="=="*40;
var x : [1..5,1..7,1..6] real;
var y : [{1..5,1..7}] real;
try {
  UnitTest.assertLessThan(x,y);
}
catch e {
  writeln(e);
  writeln(sep);
}
var x1 = [1,2,5];
var y1 = [1,2,5];
try {
  UnitTest.assertLessThan(x1,y1);
}
catch e {
  writeln(e);
  writeln(sep);
}
var x2 : [1..5,1..7] real;
var y2 : [{1..5,1..7}] real;
try {
  UnitTest.assertLessThan(x2,y2);
}
catch e {
  writeln(e);
  writeln(sep);
}
var y3 : [{1..7,1..5}] int;
try {
  UnitTest.assertLessThan(x2,y3);
}
catch e {
  writeln(e);
  writeln(sep);
}
var x4 = [1,7,6,5,8];
var y4 = [1,7,6];
try {
  UnitTest.assertLessThan(x4,y4);
}
catch e {
  writeln(e);
  writeln(sep);
}