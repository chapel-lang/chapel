record R {
  var x;
}

record R2 {
  var y : R(int);
}

var r : R2;

writeln(r);
