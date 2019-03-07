// Tracks calling a static method defined on a parent type from a child type
class Parent {
  proc type foo() {
    writeln("It's available!");
  }
}

class Child: Parent {
}

Parent.foo();
Child.foo();
