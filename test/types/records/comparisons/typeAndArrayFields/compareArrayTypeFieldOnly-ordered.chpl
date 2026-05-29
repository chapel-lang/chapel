record R {
  type t;
}

var myR = new R(t=[1..3] real);
var myR2 = new R(t=[0..2] real);

writeln(myR < myR2);
writeln(myR <= myR2);
writeln(myR > myR2);
writeln(myR >= myR2);
