use List;

class C { var x: int; }

var l = new list((int, owned C));
l.append((1, new C(1)));
l.append((2, new C(2)));
l.append((3, new C(3)));

writeln(l);
