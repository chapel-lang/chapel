use LinkedLists;

var a = new LinkedList(int);
a.append(-101,42,-17,30,2,3,7,-11,-2,10);

var b = new LinkedList(int);
b.append(-101,42,-17,30,2,3,7,-11,-2,10);

writeln(a == b);

var c = new LinkedList(int);
c.append(-101,41,-17,32,2,36,7,11,-2,100);

writeln(a == c);

var d = new LinkedList(int);
d.append(0,1,2,3,4,5,6,7,8,9);

writeln(a == d);

var e = new LinkedList(int);
var f = new LinkedList(int);

writeln(e == f);