def foo(): int {
  for i in 0..4 {
    yield i;
  }
}

def bar(): int {
  for i in [0..4] {
    yield i;
  }
}

def baz(): int {
  for i in [0..4) {
    yield i;
  }
}


for i in foo() {
  writeln("i is: ", i);
}
writeln();

for i in bar() {
  writeln("i is: ", i);
}
writeln();


for i in baz() {
  writeln("i is: ", i);
}
