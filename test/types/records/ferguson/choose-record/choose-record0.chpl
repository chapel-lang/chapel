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

A.x(0) = 1;
A.x(1) = 1;
B.x(0) = 2;
B.x(1) = 2;

// C = something creates a copy for records
var C = chooseRecord(A, B, false);
C.x(0) = 8;

var D = chooseRecord(A, B, true);
D.x(0) = 9;


writeln(A);
writeln(B);
writeln(C);
writeln(D);

