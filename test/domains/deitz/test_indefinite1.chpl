record R {
  var x: int;
}

var d : domain(R);

d += new R(2);
d += new R(3);
d += new R(4);
d += new R(5);

writeln(d);
