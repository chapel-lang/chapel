use List;

class C {
  var x : int;
  var y : real;
}

var c : C = C(1, 2.3);

var s : list of C = makeList( c );

writeln(s);
