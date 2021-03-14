record R {
  var x : int = 1;
}

record S : R {
  var y : int = 2;
}

var s : S;

writeln(s);
