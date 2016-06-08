record R {
  var x:(int, int);
}


proc chooseRecord(A:R, B:R, chooseA:bool)
{
   if chooseA then return A;
   else return B;
}

proc updateAndPrint(A:R, B:R, C:R, D:R)
{
  C.x(1) = 8;
  D.x(1) = 9;
  writeln(A);
  writeln(B);
  writeln(C);
  writeln(D);
}

var A:R;
var B:R;

A.x(1) = 1;
A.x(2) = 1;
B.x(1) = 2;
B.x(2) = 2;


updateAndPrint(A, B, chooseRecord(A, B, false), chooseRecord(A, B, true));
