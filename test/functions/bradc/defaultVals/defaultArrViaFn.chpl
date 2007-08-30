def bar() {
  var v: [i in 1..10] real = i;
  return v;
}

def foo(x = bar()) {
  writeln("x is: ", x);
}

foo();
