use LinkedLists;
use UnitTest;
var x : LinkedList(int);
var y : LinkedList(int);
x.append(1,2,3);
y.append(1,2,3);
var sep = "=="*40;
var test = new Test();
try {
  test.assertEqual(x,y);
} catch e {
  writeln(e);
  writeln(sep);
}