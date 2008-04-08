def foo() {
  for i in 1..10 do
    yield i;
}

def bar(x) {
  for i in x do
    for j in x do
      write((i,j));
  writeln();
}

bar(foo());

