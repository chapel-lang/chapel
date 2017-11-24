use List;

record R {
  var x : int;
  var y : real;
}

var r : R       = new R(1, 2.3);
var s : list(R) = makeList(r);

writeln(s);

s.destroy();
