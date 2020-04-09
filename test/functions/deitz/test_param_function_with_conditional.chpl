proc foo(i: int) param {
  if i < 1 then
    return 0;
  else
    return 2;
}

writeln(foo(3));
