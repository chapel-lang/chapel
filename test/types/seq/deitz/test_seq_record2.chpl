use List;

record R {
  var x : int;
  var y : int;
}

var s : list of R;

s.append(R(1, 2));

writeln(s);

s.prepend(R(3, 4));

writeln(s);
