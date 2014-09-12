var x : int;

proc test(i : int) ref {
  if setter then return x;
  else return (x,42);
}

test(1) = 4;
writeln(test(1));
