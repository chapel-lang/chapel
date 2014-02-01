iter foo(n: int) {
  for i in 1..n do yield i;
}

proc bar(n: int) {
  writeln("Testing size ", n);
  var A = foo(n);
  writeln("Domain = ", A.domain);
  writeln("Array = ", A);
}

serial {
  bar(1..100);
}
