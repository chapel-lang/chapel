record R {
  var x : int;
  var y : int;
}

var s : seq of R;

s = s # R(1, 2);

writeln(s);

s = R(3, 4) # s;

writeln(s);

s = s # s;

writeln(s);
