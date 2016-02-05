var x : int;

proc test(i : int) ref {
  return x;
}
proc test(i : int) {
  return (x,42);
}

test(1) = 4;
writeln(test(1));
