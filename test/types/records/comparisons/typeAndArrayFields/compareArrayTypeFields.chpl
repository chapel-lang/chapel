config const lo = 0;

record R {
  type t;
  var A: t;
}

var myR = new R(t=[lo..#3] real);
var myR2 = new R(t=[0..2] real);

writeln(myR == myR2);
writeln(myR != myR2);
