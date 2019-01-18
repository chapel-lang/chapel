proc foo(param small): (int, int, int) {
  if small then
    return (1,2);
  else
    return (1,2,3,4);
}

writeln(foo(true));
writeln(foo(false));

