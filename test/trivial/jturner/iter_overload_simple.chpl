iter myiter() {
  for myiter_iter in 1..3 {
    yield myiter_iter;
  }
}

class Parent {
  iter foo(k:int) {
    yield 2;
  }
}

class Child : Parent {
  iter foo(k:int) {
    for l in myiter() {
      yield k+l+100;
    }
  }
}

var c:Parent = new Child();

for m in c.foo(10) {
  writeln(m);
}

delete c;
