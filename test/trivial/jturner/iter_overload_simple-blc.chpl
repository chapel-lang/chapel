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
  override iter foo(k:int) {
    for l in myiter() {
      yield k+l+100;
    }
  }
}

class GrandChild : Child {
  override iter foo(k:int) {
    yield 3;
  }
}

var c = new owned Child();

for m in c.foo(10) {
  writeln(m);
}


c = new owned GrandChild();

for m in c.foo(10) {
  writeln(m);
}
