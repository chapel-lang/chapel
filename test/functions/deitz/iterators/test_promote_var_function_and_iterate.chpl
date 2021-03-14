var A: [1..3] int;

proc foo(i: int) ref {
  writeln("foo called in setter context");
  return A(i);
}

proc foo(i: int) {
  return A(i);
}

for t in foo(1..3) {
  writeln(t);
}

for i in foo(1..3) {
  i = 1;
}

for t in zip(foo(1..3), 1..3) {
  writeln(t);
}

for t in zip(foo(1..3), 1..3) {
  t(1) = t(1) + 1;
  writeln(t);
}

iter iterate() ref {
  for j in foo(1..3) {
    yield j;
  }
}

for i in iterate() {
  writeln(i);
}

for i in iterate() {
  i = i + 1;
  writeln(i);
}

iter iter2() ref {
  for i in 1..3 {
    yield foo(i);
  }
}

for i in iter2() {
  writeln(i);
}

for i in iter2() {
  i = i + 1;
  writeln(i);
}

