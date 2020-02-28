use List;

class C {
  var i: int;
}

var l1 = new list(shared C);
l1.append(new shared C(1));
l1.append(new shared C(2));

var l2 = new list(shared C);
l2.append(new shared C(3));
l2.append(new shared C(4));

l1.insert(2, new shared C(5));
var c = l1[3];
c.i *= 10;

writeln(c);
writeln(l1);
writeln(l2);
