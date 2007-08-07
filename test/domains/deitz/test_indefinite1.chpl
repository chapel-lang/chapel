record R {
  var x: int;
}

var d : domain(R);

d += R(2);
d += R(3);
d += R(4);
d += R(5);

writeln(d);
