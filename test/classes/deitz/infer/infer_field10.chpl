record R {
  var x;
}

record R2 {
  var y : R(2);
}

var r : R2;

writeln(r);
