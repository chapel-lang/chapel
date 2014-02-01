proc foo(i : int) {
  writeln(i);
}

var s : [1..4] int =( 1, 2, 3, 4 );

serial {
  foo(s);
}
