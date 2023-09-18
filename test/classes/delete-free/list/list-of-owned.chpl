use List;

class C {
  var i: int;
}

var l1 = new list(owned C);
l1.pushBack(new owned C(1));
l1.pushBack(new C(2));

var l2 = new list(owned C);
l2.pushBack(new owned C(3));
l2.pushBack(new C(4));

l1.insert(1, new C(5));
var c = l1.popBack();

writeln(c);
writeln(l1);
writeln(l2);
