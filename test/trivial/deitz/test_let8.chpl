function foo(inout a : integer) : integer {
  a += 1;
  return a;
}

var a = 1;

writeln(foo(a) - foo(a));

a = 1;

writeln(foo(a) - let b = foo(a) in b);
