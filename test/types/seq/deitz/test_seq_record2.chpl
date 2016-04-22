use List;

record R {
  var x : int;
  var y : int;
}

var s : list(R);

s.append(new R(1, 2));

writeln(s);

s.prepend(new R(3, 4));

writeln(s);

s.destroy();
