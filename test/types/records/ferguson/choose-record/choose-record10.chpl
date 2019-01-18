record R {
  var x:(int, int);
}


proc chooseRecord(A:R, B:R, chooseA:bool)
{
   if chooseA then return A;
   else return B;
}

var A:R;
var B:R;

A.x(1) = 1;
A.x(2) = 1;
B.x(1) = 2;
B.x(2) = 2;

ref C = chooseRecord(A, B, false);
C.x(1) = 8;

ref D = chooseRecord(A, B, true);
D.x(1) = 9;


writeln(A);
writeln(B);
writeln(C);
writeln(D);

