
proc chooseArray(A:[] int, B:[] int, chooseA:bool)
{
  var AA => A;
  var BB => B;
  if chooseA then return AA;
  else return BB;
}

var A:[1..10] int;
var B:[1..10] int;

A = 1;
B = 2;

var C => chooseArray(A, B, false);
C[1] = 8;

var D => chooseArray(A, B, true);
D[1] = 9;


writeln(A);
writeln(B);
writeln(C);
writeln(D);

