record R {
  type t;
  param x: int;
}

var myR = new R(t=int, x=42);
writeln(myR == myR);
writeln(myR != myR);
writeln(myR < myR);
writeln(myR <= myR);
writeln(myR > myR);
writeln(myR >= myR);
