use LinkedLists;
use UnitTest;
var x : LinkedList(int);
var y : LinkedList(int);
x.append(1,2,3);
y.append(1,2,3);
var sep = "=="*40;
try {
  UnitTest.assertEqual(x,y);
} catch e {
  writeln(e);
  writeln(sep);
}