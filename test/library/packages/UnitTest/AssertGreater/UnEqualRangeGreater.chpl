use UnitTest;
use Reflection;
var x = ..7;
var y = ..6;
var test = new Test();
var sep = "=="*40;
try {
  test.assertGreaterThan(x,y);
} catch e {
  writeln(e);
  writeln(sep);
}