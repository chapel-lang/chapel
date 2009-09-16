use List;

class C {
  var x : int;
  var y : real;
}

var c1 : C = new C(1, 2.3), c2 : C = new C(2, 3.4);

var s : list(C) = makeList( c1, c2 );

writeln(s);

s.concat(makeList(c1, c2));

writeln(s);

delete c1;
delete c2;
