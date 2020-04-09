
proc chooseArray(A:[] int, B:[] int, chooseA:bool)
{
   if chooseA then return A;
   else return B;
}

var A:[1..10] int;
var B:[1..10] int;

A = 1;
B = 2;

// C = something creates a copy for arrays
var C = chooseArray(A, B, false);
C[1] = 8;

var D = chooseArray(A, B, true);
D[1] = 9;


writeln(A);
writeln(B);
writeln(C);
writeln(D);

