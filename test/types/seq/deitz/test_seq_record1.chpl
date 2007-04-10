record R {
  var x : int;
  var y : real;
}

var r : R = R(1, 2.3);

var s : seq of R = _seq( r );

writeln(s);
