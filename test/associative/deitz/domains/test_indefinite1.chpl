use printHelp;

record R {
  var x: int;
}

var d : domain(R);

d += new R(2);
d += new R(3);
d += new R(4);
d += new R(5);

writelnSorted(d);

operator R.<(x1: R, x2: R) {
  return x1.x < x2.x;
}
