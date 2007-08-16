use List;

record R {
  var x : int;
  var y : real;
}

var r : R = R(1, 2.3);

var s : list of R = makeList( r );

writeln(s);
