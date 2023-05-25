var n: int;

proc foo(i: int) {
  n = n + 1;
  writeln(i);
}

proc bar(i: int) do return n + i;

serial {
  foo(bar(1..10));
}

