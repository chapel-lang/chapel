var x : int;

proc test(i : int) var {
  if setter then return x;
  else return (x,42);
}

test(1) = 4;
writeln(test(1));
