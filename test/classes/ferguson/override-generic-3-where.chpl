class Parent {
  proc foo(param x) {
    writeln("Parent ", x);
  }
}

class Child : Parent {
  proc foo(param x) where x == 1 || x == 2 {
    writeln("Child ", x);
  }
}

class ChildSub : Child {
  proc foo(param x) where x == 2 {
    writeln("ChildSub ", x);
  }
}

var x = new ChildSub();
x.foo(1);
x.foo(2);
x.foo(3);
