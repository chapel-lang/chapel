record R {
  var x:(int, int);
}

proc chooseRecord(ref A:R, ref B:R, chooseA:bool) ref
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

// var C = SomeRecord creates a deep copy
var C = chooseRecord(A, B, false);
C.x(0) = 8;

var D = chooseRecord(A, B, true);
D.x(0) = 9;


writeln(A);
writeln(B);
writeln(C);
writeln(D);

