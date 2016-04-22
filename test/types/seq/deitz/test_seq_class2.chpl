use List;

class C {
  var x : int;
  var y : real;
}

var c1 : C       = new C(1, 2.3);
var c2 : C       = new C(2, 3.4);

var l1 : list(C) = makeList(c1, c2);
var l2 : list(C) = makeList(c1, c2);

writeln(l1);

l1.concat(l2);

writeln(l1);

l2.destroy();
l1.destroy();

delete c1;
delete c2;
