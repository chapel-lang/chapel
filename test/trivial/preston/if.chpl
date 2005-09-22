-- let's look at conditional expressions versus if statements

function zap(a: integer, b : integer) : integer {
  if (if a == b then a < 10 else a >= 10) then return 1;
  else return 2;
}


writeln(zap(1, 2));
