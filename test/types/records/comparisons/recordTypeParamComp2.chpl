record R {
  type t;
  param x: int;
}

var myR = new R(t=int, x=42);
var myR2 = new R(t=int, x=45);
writeln(myR < myR2);
writeln(myR <= myR2);
writeln(myR > myR2);
writeln(myR >= myR2);
writeln(myR == myR2);
writeln(myR != myR2);
