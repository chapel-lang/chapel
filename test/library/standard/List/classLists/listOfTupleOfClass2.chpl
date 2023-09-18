use List;

class C { var x: int; }

var l = new list((int, owned C));
l.pushBack((1, new C(1)));
l.pushBack((2, new C(2)));
l.pushBack((3, new C(3)));

writeln(l);
