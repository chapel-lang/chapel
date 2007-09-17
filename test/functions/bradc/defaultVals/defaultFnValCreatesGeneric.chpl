def bar() {
  return 1;
}

def foo(x = bar()) {
  writeln("x is: ", x);
}

foo();
foo(1.0);

