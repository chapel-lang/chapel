def bar() {
  var v: [1..10] real = [i in 1..10] i;
  return v;
}

def foo(x = bar()) {
  writeln("x is: ", x);
}

foo();
