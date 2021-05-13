record R {
  var x, y: int;
}

var R1 = new R(x=1,y=2);
var R2 = new R(x=3,y=4);

writeln(R1,R2);

R1<=>R2;

writeln(R1,R2);

operator R.<=>(ref x: R, ref y: R) {
  x.x <=> y.y;
  x.y <=> y.x;
}
