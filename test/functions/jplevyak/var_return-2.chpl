var x : integer = 1;
var y : integer = 3;
var z : integer = 2;
function f() var : integer { 
  return (if (x > y) then x else (if (y > z) then y else z));
}
f() = 7;
writeln("x = ", x," y = ", y, " z = ", z);
