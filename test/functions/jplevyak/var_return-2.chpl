var x : int = 1;
var y : int = 3;
var z : int = 2;
proc f() ref : int { 
  return (if (x > y) then x else (if (y > z) then y else z));
}
f() = 7;
writeln("x = ", x," y = ", y, " z = ", z);
