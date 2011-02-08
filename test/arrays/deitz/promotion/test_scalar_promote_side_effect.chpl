var n: int;

proc foo(i: int) {
  n = n + 1;
  writeln(i);
}

proc bar(i: int) return n + i;

serial true {
  foo(bar(1..10));
}

