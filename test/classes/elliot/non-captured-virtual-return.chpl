class Parent {
  proc foo() { return (3, 3); }
}

class Child: Parent {
  proc foo() { return (1,1); }
}

var child: Parent = new Child();
child.foo();
delete child;
