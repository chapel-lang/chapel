use List;

class C {
  var i: int;
}

var l1 = new list(owned C);
l1.pushBack(new owned C(1));
l1.pushBack(new C(2));

var l2 = l1; // error, l1's elements are non-nillable, this makes them nil

writeln(l1);
writeln(l2);
