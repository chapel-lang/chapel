use LinkedLists;
use UnitTest;
var x : LinkedList(int);
var y : LinkedList(int);
x.append(1,2,3);
y.append(1,2,3);
UnitTest.assertEqual(x,y);