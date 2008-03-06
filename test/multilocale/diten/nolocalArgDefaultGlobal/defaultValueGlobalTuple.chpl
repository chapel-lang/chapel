var t = (1, 2);
def foo(a=t(1)) {
  return a;
}

writeln(foo());
t(1) = 2;
writeln(foo());
