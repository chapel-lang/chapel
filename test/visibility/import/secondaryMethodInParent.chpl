module M {
  use Child;

  proc C.foo() {
    writeln("In C.foo()");
  }

  module Child {
    class C {
    }
  }

  module Child2	{
    proc main() {
      import M.Child.C;  // import class C, but nothing in my parent module
      var myC = new C();
      myC.foo();
    }
  }
}
