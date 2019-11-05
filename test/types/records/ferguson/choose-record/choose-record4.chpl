record R {
  var x:(int, int);
}


proc chooseRecord(ref A:R, ref B:R, chooseA:bool) ref
{
   if chooseA then return A;
   else return B;
}

proc updateAndPrint(A:R, B:R, C:R, D:R)
{
  C.x(0) = 8;
  D.x(0) = 9;
  writeln(A);
  writeln(B);
  writeln(C);
  writeln(D);
}

var A:R;
var B:R;

A.x(0) = 1;
A.x(1) = 1;
B.x(0) = 2;
B.x(1) = 2;

updateAndPrint(A, B, chooseRecord(A, B, false), chooseRecord(A, B, true));
