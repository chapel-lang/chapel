proc isDegen(r) {
  return r.low > r.high;
}

var r1 = 0..10 by 2;
var r2 = 9..1 by 2;

writeln((isDegen(r1), isDegen(r2), isDegen(r1(r2))));
