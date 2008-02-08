def foo() {
  for i in 1..10 do
    yield i;
}

def bar(x) {
  for i in x do
    writeln(i);
  for i in x do
    writeln(i);
}

bar(foo());
