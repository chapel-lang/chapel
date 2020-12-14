// Tests the behavior when listing a subclass in the only list of a used module
// that defines a tertiary method on a parent type.
module definesClass {
  class Parent {
    var a: int;
  }

  class Child: Parent {
    var b: int;
  }
}

module definesTertiary {
  use definesClass;

  proc Parent.tertiaryMethod() {
    writeln("In inherited tertiary method");
  }
}

module User {
  use definesClass;
  // The module used below defines a tertiary method on `Parent`, the parent
  // type of `Child`.  Because of inheritance, this method is also now defined
  // on instances of `Child` in that scope, but we've decided that the
  // limitation clause only applies to the immediate type it is defined on,
  // rather than applying to all parent types of the type listed.
  use definesTertiary only Child;

  proc main() {
    var x = new owned Child(2, 3);
    x.tertiaryMethod(); // This means this should be an error.
  }
}
