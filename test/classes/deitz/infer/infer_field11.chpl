record R {
  var x;
}

record R2 {
  var y: R(2);
}

record R3 {
  var y: R(2.0);
}

var r: R2;

writeln(r);

var rr: R3;
writeln(rr);
