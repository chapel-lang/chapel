proc foo(i) {
  if i < 1 then return 1; else return foo(i-1);
}

writeln(foo(4));
