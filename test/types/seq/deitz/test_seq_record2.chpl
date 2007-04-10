record R {
  var x : int;
  var y : int;
}

var s : seq of R;

s = _seqcat(s, R(1, 2));

writeln(s);

s = _seqcat(R(3, 4), s);

writeln(s);

s = _seqcat(s, s);

writeln(s);
