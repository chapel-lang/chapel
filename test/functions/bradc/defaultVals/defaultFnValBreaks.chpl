proc bar() {
  return 1;
}

proc foo(x = bar()) {
  writeln("x is: ", x);
}

foo();
