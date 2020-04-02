var t = (1, 2);
proc foo(a=t(0)) {
  return a;
}

writeln(foo());
t(0) = 2;
writeln(foo());
