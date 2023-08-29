
proc chooseArray(ref A:[] int, ref B:[] int, chooseA:bool)
{
  ref AA = A;
  ref BB = B;
  if chooseA then return AA;
  else return BB;
}

proc updateAndPrint(ref A:[], ref B:[], C:[], D:[])
{
  A[1] = 8;
  B[1] = 9;
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
