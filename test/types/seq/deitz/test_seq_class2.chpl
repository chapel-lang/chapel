use List;

class C {
  var x : int;
  var y : real;
}

var c1 : C = C(1, 2.3), c2 : C = C(2, 3.4);

var s : list of C = makeList( c1, c2 );

writeln(s);

s.concat(makeList(c1, c2));

writeln(s);
