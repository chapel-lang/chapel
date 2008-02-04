record R {
  var x;
}

record R2 {
  var y: R(int);
}

record R3 {
  var y: R(real);
}

var r: R2;

writeln(r);

var rr: R3;
writeln(rr);
