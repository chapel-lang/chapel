record R {
  var x : integer;
  var y : float;
}

var r : R = R(1, 2.3);

var s : seq of R = (/ r /);

writeln(s);
