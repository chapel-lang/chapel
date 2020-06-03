use List;

class C {
  var i: int;
}

var l1 = new list(owned C);
l1.append(new owned C(1));
l1.append(new C(2));

var l2 = new list(owned C);
l2.append(new owned C(3));
l2.append(new C(4));

l1.insert(1, new C(5));
var c = l1.pop();

writeln(c);
writeln(l1);
writeln(l2);
