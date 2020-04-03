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

chooseRecord(A, B, false).x(0) = 8;

chooseRecord(A, B, true).x(0) = 9;


writeln(A);
writeln(B);

