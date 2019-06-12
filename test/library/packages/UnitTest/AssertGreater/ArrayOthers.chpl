use UnitTest;
var sep = "=="*40; 
var x1 : [1..5,1..7,1..6] real;
var y1 : [{1..5,1..7}] real;
try {
  UnitTest.assertGreaterThan(x1,y1);
}
catch e {
  writeln(e);
  writeln(sep);
}
var x2 = [1,2,5];
var y2 = [1,2,5];
try {
  UnitTest.assertGreaterThan(x2,y2);
}
catch e {
  writeln(e);
  writeln(sep);
}
var x3 : [1..5,1..7] real;
var y3 : [{1..5,1..7}] real;
try {
  UnitTest.assertGreaterThan(x3,y3);
}
catch e {
  writeln(e);
  writeln(sep);
}
var x4 : [1..5,1..7] real;
var y4 : [{1..7,1..5}] int;
try {
  UnitTest.assertGreaterThan(x4,y4);
}
catch e {
  writeln(e);
  writeln(sep);
}
var x5 = [1,7,6,5,8];
var y5 = [1,7,6];
try {
  UnitTest.assertGreaterThan(x5,y5);
}
catch e {
  writeln(e);
  writeln(sep);
}