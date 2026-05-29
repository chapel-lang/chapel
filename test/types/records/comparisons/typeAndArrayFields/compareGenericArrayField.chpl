record R {
  var A;
}

var myR = new R(A=[i in 1..3] (i + 0.0));
var myR2 = new R(A=[i in 0..2] (i + 1.0));

writeln(myR == myR2);
writeln(myR != myR2);
