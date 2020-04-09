// Tracks calling a static method defined on a parent type from a child type
class Parent {
  proc type foo() param {
    return 3;
  }
}

class Child: Parent {
}

writeln(Parent.foo());
writeln(Child.foo());
