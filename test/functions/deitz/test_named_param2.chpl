proc foo(i : int, j : int) {
  writeln(i);
  writeln(j);
}

foo(1,2);
foo(i=1,j=2);
foo(2,1);
foo(j=1,i=2);
