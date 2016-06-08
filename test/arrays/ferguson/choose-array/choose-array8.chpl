
proc chooseArray(A:[] int, B:[] int, chooseA:bool)
{
  if chooseA then return A;
  else return B;
}

var A:[1..10] int;
var B:[1..10] int;

A = 1;
B = 2;

chooseArray(A, B, false)[1] = 8;

chooseArray(A, B, true)[1] = 9;


writeln(A);
writeln(B);

