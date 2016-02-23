class Parent {
  proc foo(param x) {
    writeln("Parent ", x);
  }
}

class Child : Parent {
  proc foo(param x) {
    writeln("Child ", x);
  }
}

class ChildSub : Child {
  proc foo(param x) {
    writeln("ChildSub ", x);
  }
}

var x = new ChildSub();
x.foo(1);
x.foo(2);
x.foo(3);
