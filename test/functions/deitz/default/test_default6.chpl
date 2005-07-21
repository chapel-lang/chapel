function foo(x : integer = 4) {
  writeln("foo of ", x);
  bar(x);
  bar();
}

function bar(x : float = 2.0) {
  writeln("bar of ", x);
}

foo();
