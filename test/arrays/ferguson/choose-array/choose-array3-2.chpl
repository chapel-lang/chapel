
proc chooseArray(A:[] int, B:[] int, chooseA:bool)
{
   if chooseA then return A;
   else return B;
}

proc updateAndPrint(ref A:[], ref B:[], ref C:[], ref D:[])
{
  C[1] = 8;
  D[1] = 9;
  writeln(A);
  writeln(B);
  writeln(C);
  writeln(D);
}

var A:[1..10] int;
var B:[1..10] int;

A = 1;
B = 2;

updateAndPrint(A, B, chooseArray(A, B, false), chooseArray(A, B, true));
