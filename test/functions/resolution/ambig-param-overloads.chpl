record R {
  param flag;
  var x: int = 42;
}

proc R.getX where flag return x;
proc R.getX param where !flag return 0;
proc R.getX param where !flag return 0;

var myR1 = new R(false);
var myR2 = new R(true);

writeln(myR1.getX);
writeln(myR2.getX);
