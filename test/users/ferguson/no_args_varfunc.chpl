proc myfun(x ...?k) {
  writeln("myfun called");
  for param i in 1..k {
    writeln(x(i));
  }
  writeln("done myfun");
}

myfun();

