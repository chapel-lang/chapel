// let's look at conditional expressions versus if statements

proc zap(a: int, b : int) : int {
  if (if a == b then a < 10 else a >= 10) then return 1;
  else return 2;
}


writeln(zap(1, 2));
