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

var x1 = new Parent();

x1.foo(1);
x1.foo(2);
x1.foo(3);

delete x1;



writeln();




var x2 = new Child();

x2.foo(1);
x2.foo(2);
x2.foo(3);

delete x2;



writeln();





var x3 = new ChildSub();

x3.foo(1);
x3.foo(2);
x3.foo(3);

delete x3;
